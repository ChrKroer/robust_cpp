import robustPortfolio_gen
import robustQP_gen
import robustSVM_gen
import robustLP_gen
import subprocess
import os


f = open('Experiment_Log.json', 'w')

#1 -> robustPort, 2 -> robustQP, 3 -> robustSVM
mode = 1

#parameters other than file name/location
otherCommands = ["-a", "pessimization"]

paramSettings = [(4,[10,4,3,90,0.95,1,False])]

#list of (times_run, list_of_params)

#1 -> n,m,rfr,p,sig,lamb,robust_return
#2 -> n,m,Kmax, round_digits
#3 -> n,m,C,perturb_lvl

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
		# robust return constraint flag (False: only include the nominal constraint. True: includ ethe robust constraint also)
		robust_return = b[6]

		filename = "temp_file_Port"
		for i in b:
			filename += "_" + str(i)

		for i in xrange(0,a):
			robustPortfolio_gen.robustPort(filename=filename, savedir='./',  # '',#
	           n=n, m=m, rfr=rfr, p=p,
	           lamb=lamb, sig=sig,
	           robust_return=robust_return)

			f.write(subprocess.check_output(["../build/robust_cpp", "-m", filename + ".mps"]  + otherCommands))

			os.remove(filename + ".mps")
			os.remove(filename + ".json")

elif mode == 2:
	for (a,b) in paramSettings:
		n = b[0]
		m = b[1]
		Kmax = b[2]
		round_digits = b[3]


		filename = "temp_file_QP"
		for i in b:
			filename += "_" + str(i)

		for i in xrange(0,a):
			robustQP(filename=None,
	         savedir='./',
	         n=n, m=m, Kmax=Kmax, round_digits=roung_digits)

			f.write(subprocess.check_output(["../build/robust_cpp", "-m", filename + ".mps"]  + otherCommands))

			os.remove(filename + ".mps")
			os.remove(filename + ".json")

elif mode == 3:
	for (a,b) in paramSettings:
		n = b[0]
		m = b[1]
		C = b[2]
		perturb_lvl = b[3]

		filename = "temp_file_SVM"
		for i in b:
			filename += "_" + str(i)

		for i in xrange(0,a):
			robustSVM(filename=filename, savedir=./,
          		n=n, m=m, C=C,
          		perturb_lvl=perturb_lvl)

			f.write(subprocess.check_output(["../build/robust_cpp", "-m", filename + ".mps"] + otherCommands))

			os.remove(filename + ".mps")
			os.remove(filename + ".json")


