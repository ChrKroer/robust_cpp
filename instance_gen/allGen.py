import robustPortfolio_gen
import robustQP_gen
import robustSVM_gen
import robustLP_gen
import subprocess
import os


f = open('Experiment_Log.json', 'w')
paramSettings = [(4,[10,4,3,90,0,1,False])]
for (a,b) in paramSettings:
	n = b[0]
	# number of factors
	m = b[1]
	# risk-free rate
	rfr = b[2]
	# number of samples
	p = b[3]
	# significance level for uncertainty sets
	sig = 0.95
	# balance between maximizing return and minimizing risk
	lamb = b[5]
	# robust return constraint flag (False: only include the nominal constraint. True: includ ethe robust constraint also)
	robust_return = b[6]

	filename = "temp_file_Port"
	for i in b:
		filename += "_" + str(i)

	filename

	for i in xrange(0,a):
		robustPortfolio_gen.robustPort(filename=filename, savedir='./',  # '',#
           n=n, m=m, rfr=rfr, p=p,
           lamb=lamb, sig=sig,
           robust_return=robust_return)

		f.write(subprocess.check_output(["../build/robust_cpp", "-a", "pessimization", "-m", filename + ".mps"]))

		os.remove(filename + ".mps")
		os.remove(filename + ".json")
