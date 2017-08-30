# -*- coding: utf-8 -*-
"""
Created on Tue May  9 00:14:22 2017

@author: hnh
"""


import numpy as np
from numpy.linalg import norm
import sklearn.datasets
from sklearn.svm import SVC
import scipy.stats
import gurobipy
import pandas as pd
import json
import os


class MyEncoder(json.JSONEncoder):
  def default(self, obj):
    if isinstance(obj, np.integer):
      return int(obj)
    elif isinstance(obj, np.floating):
      return float(obj)
    elif isinstance(obj, np.ndarray):
      return obj.tolist()
    else:
      return super(MyEncoder, self).default(obj)


def namer(newname='PortOpt_0', savedir='../instances'):
  if(len(savedir) == 0):
    names = os.listdir()
    names = [name.strip('.mps')
             for name in names if name.split('.')[-1] == 'mps']
  else:
    if not os.path.exists(savedir):
      names = set()
    else:
      names = os.listdir(savedir)
      names = [name.strip('.mps')
               for name in names if name.split('.')[-1] == 'mps']
  savedir += '/'
  suffix = newname.split('_')[-1]
  try:
    suffix = str(int(suffix) + 1)
    newname = '_'.join(newname.split('_')[:-1]) + '_' + suffix
  except:
    newname += '_0'
  while newname in names:
    suffix = newname.split('_')[-1]
    try:
      suffix = str(int(suffix) + 1)
      newname = '_'.join(newname.split('_')[:-1]) + '_' + suffix
    except:
      newname += '_0'
  return newname


def robustSVM(filename=None, savedir='../instances',
              n=20, m=100,
              C=None,
              perturb_lvl=0.1):
  """
  Generates a robust portfolio instance based on a factor model.
  Returns the nominal MPS and robust constraint json files.
  Inputs:
      n: number of features
      m: number of samples
      C: tradeoff between regularization and objective (decrease for more regularization)
          defaults to an automatically selected C from grid search on linear SVM of nominal problem.
      perturb_lvl: how much to perturb ground truth features by.
  """
  if(filename == None):
    filename = 'robustSVM_n%i_m%i_inst' % (n, m)
    filename = namer(filename, savedir)

  if(len(savedir) > 0):
    savedir += '/'
    if not os.path.exists(savedir):
      os.makedirs(savedir)

  X, y = sklearn.datasets.make_classification(n_samples=m, n_features=n)
  X = X.transpose()
  y = 2 * y - 1
  Xerror = np.random.randn(n, m) / np.sqrt(n)
  Xerror = Xerror / np.maximum(1, norm(Xerror, axis=0).reshape((1, m)))
  X0 = X + perturb_lvl * Xerror

  X0Y = X0 * y.reshape((1, m))
  SVec = SVC(kernel='linear')

  if(C is None):
    Clogrange = [-5, -4, -3, -2, -1, 0, 1, 2]
    bestC = 10**Clogrange[0]
    bestfit = np.inf
    for exp in Clogrange:
      Ctest = 10**exp
      SVec.set_params(C=Ctest)
      SVec.fit(X0.transpose(), y)
      currentfit = SVec.score(X0.transpose(), y)
      #print(SVec.get_params()['C'], currentfit)
      if(currentfit < bestfit):
        bestC = Ctest
        bestfit = currentfit
    C = bestC

  modname = filename
  mod = gurobipy.Model(modname)
  mod.setParam('OutputFlag', 0)

  gamma = mod.addVar(name='gamma', obj=1.0 / 2, lb=-gurobipy.GRB.INFINITY)
  alpha = pd.Series(mod.addVars(
      range(m), name='alpha', lb=0, ub=C), index=range(m))

  mod.addConstr(y.dot(alpha) == 0, name='equality')
  mod.addQConstr(alpha.transpose().dot(X0Y.transpose()).dot(
      X0Y).dot(alpha) <= gamma, name='kernel')

  mod.update()
  mod.write(savedir + modname + '.mps')

  robustData = []
  k = 2 * n
  constrData = dict()
  constrData['name'] = 'kernel'
  con = mod.getQConstrs()[0]
  assert(con.QCName == constrData['name'])
  constrData['id'] = con._qconstrno

  constrData['type'] = 'quadratic'

  vnames = [int(v._colno) for v in alpha]
  constrData['vars'] = vnames

  constrData['base_matrix'] = dict()
  constrData['base_matrix']['nrows'] = X0Y.shape[0]
  constrData['base_matrix']['ncols'] = X0Y.shape[1]
  constrData['base_matrix']['format'] = 'dense'
  constrData['base_matrix']['vals'] = X0Y

  constrData['uncertainty'] = dict()
  constrData['uncertainty']['type'] = 'L2ball'
  constrData['uncertainty']['radius'] = 1
  # of the format [(k,val) for k in K]
  #[] means the centre is the zero vector
  constrData['uncertainty']['centre'] = []
  constrData['uncertainty']['data'] = []

  for j in range(k):
    P = np.random.randn(n, m)
    P = perturb_lvl * P / norm(P, axis=0)
    P = np.around(P, decimals=9) * y
    matrix = dict()
    matrix['nrows'] = P.shape[0]
    matrix['ncols'] = P.shape[1]
    matrix['format'] = 'dense'
    matrix['vals'] = P
    constrData['uncertainty']['data'].append(matrix)

  robustData.append(constrData)
  constrData['certain_variable_coefficient'] = [-1]
  constrData['certain_variable_name'] = ['gamma']
  constrData['certain_variable_index'] = [gamma._colno]

  with open(savedir + modname + '.json', 'w') as f:
    json.dump(robustData, f, cls=MyEncoder, indent=4)


filename = None
savedir = '../instances'  # ''#
n = 10
m = 30
C = 2.0
perturb_lvl = 0.1

robustSVM(filename=filename, savedir=savedir,
          n=n, m=m,
          C=C,
          perturb_lvl=perturb_lvl)

#mod = gurobipy.read('robustSVM_n10_m30_inst_0')
# mod.optimize()
