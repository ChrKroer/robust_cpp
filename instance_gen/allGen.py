import robustPortfolio_gen
import robustQP_gen
import robustSVM_gen
import robustLP_gen
import subprocess
import os


f = open('Experiment_Log.json', 'w')
f.write('[')
#1 -> robustPort, 2 -> robustQP, 3 -> robustSVM
mode = 1

directory = './'

names = os.listdir(directory)
names = [name.strip('.mps') for name in names if name.split('.')[-1] == 'mps']

deleteInstances = True

#parameters other than file name/location
otherCommands = [["-a", "pessimization"], ["-a", "regret", "--regret_minimizer", "ftpl", "--stepsize", "0.2"]]

#parameters of instance generation
paramSettings = [(4,[10,4,3,90,0.95,1,False])]

#list of (times_run, list_of_params)

#1 -> n,m,rfr,p,sig,lamb,robust_return
#2 -> n,m,Kmax, round_digits
#3 -> n,m,C,perturb_lvl

isFirst = True

if mode == 1:
	for (a,b) in paramSettings:
		n = b[0]
		# number of factors
		m = b[1]
		# risk-free rate
		rfr = b[2]
		# number of samples
		p = b[3]
		# significance level for uncertainty sets
		sig = b[4]
		# balance between maximizing return and minimizing risk
		lamb = b[5]
		# robust return constraint flag (False: only include the nominal constraint. True: include the robust constraint also)
		robust_return = b[6]
		if(deleteInstances):
			filename = "temp_file_Port"
		else: 
			filename = "PortOpt"

		for i in b:
			filename += "_" + str(i)
		
		tfn = filename

		index = 0

		for i in xrange(0,a):
			if(not deleteInstances):
				index += 1
				tfn = filename + "_v" + str(index)
				while tfn in names:
					index += 1
					tfn = filename + "_v" + str(index)

			robustPortfolio_gen.robustPort(filename=tfn, savedir=directory,  # '',#
	           n=n, m=m, rfr=rfr, p=p,
	           lamb=lamb, sig=sig,
	           robust_return=robust_return)

			for oc in otherCommands:
				if isFirst:
					isFirst = False
				else: 
					f.write(", ")
				f.write(subprocess.check_output(["../build/robust_cpp", "-m", directory + tfn + ".mps"]  + oc))

			if(deleteInstances):
				os.remove(directory + tfn + ".mps")
				os.remove(directory + tfn + ".json")

elif mode == 2:
	for (a,b) in paramSettings:
		n = b[0]
		m = b[1]
		Kmax = b[2]
		round_digits = b[3]

		if(deleteInstances):
			filename = "temp_file_QP"
		else: 
			filename = "QPOpt"

		for i in b:
			filename += "_" + str(i)

		tfn = filename

		index = 0
		
		for i in xrange(0,a):
			if(not deleteInstances):
				index += 1
				tfn = filename + "_v" + str(index)
				while tfn in names:
					index += 1
					tfn = filename + "_v" + str(index)

			robustQP(filename=tfn,
	         savedir=directory,
	         n=n, m=m, Kmax=Kmax, round_digits=roung_digits)

			for oc in otherCommands:
				if isFirst:
					isFirst = False
				else: 
					f.write(", ")
				f.write(subprocess.check_output(["../build/robust_cpp", "-m", directory + tfn + ".mps"]  + oc))
			if(deleteInstances):
				os.remove(directory + tfn + ".mps")
				os.remove(directory + tfn + ".json")

elif mode == 3:
	for (a,b) in paramSettings:
		n = b[0]
		m = b[1]
		C = b[2]
		perturb_lvl = b[3]


		if(deleteInstances):
			filename = "temp_file_SVM"
		else: 
			filename = "SVMOpt"

		for i in b:
			filename += "_" + str(i)
		index = 0

		tfn = filename

		for i in xrange(0,a):
			if(not deleteInstances):
				index += 1
				tfn = filename + "_v" + str(index)
				while tfn in names:
					index += 1
					tfn = filename + "_v" + str(index)
				filename = tfn

			robustSVM(filename=tfn, savedir=directory,
          		n=n, m=m, C=C,
          		perturb_lvl=perturb_lvl)
			
			for oc in otherCommands:
				if isFirst:
					isFirst = False
				else: 
					f.write(", ")
				f.write(subprocess.check_output(["../build/robust_cpp", "-m", directory + tfn + ".mps"]  + oc))

			if(deleteInstances):
				os.remove(directory + tfn + ".mps")
				os.remove(directory + tfn + ".json")


f.write(']')
f.close()