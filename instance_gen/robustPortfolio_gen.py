# -*- coding: utf-8 -*-
"""
Created on Tue May  2 23:01:37 2017

@author: hnh
"""

import numpy as np
from sklearn import linear_model
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


def robustPort(filename=None, savedir='../instances',
               n=10, m=4, rfr=3, p=90,
               lamb=1, sig=0.95,
               robust_return=False):
  """
  Generates a robust portfolio instance based on a factor model.
  Returns the nominal MPS and robust constraint json files.
  Inputs:
      n: number of assets
      m: number of factors
      rfr: risk free rate (used to generate return means)
      p: number of past samples to estimate data from.
      sig: level of significance to construct linear constraint uncertainty set.
      lamb: emphasis level on maximizing return vs minimizing risk.
          Higher means more emphasis on return.
      robust_return: set to True if you want the robust linear constraint processed
          and in the nominal MPS file.
  """

  if(filename == None):
    filename = 'PortOpt_n%i_m%i_inst' % (n, m)
    filename = namer(filename, savedir)

  if(len(savedir) > 0):
    savedir += '/'
    if not os.path.exists(savedir):
      os.makedirs(savedir)

  F = np.random.randn(m, m) / m
  F = F.transpose().dot(F)
  eigF = np.linalg.eigvalsh(F)
  F = F + max(0, eigF[-1] / 19 - 20 * eigF[0] / 19) * np.eye(m)
  eigF = np.linalg.eigvalsh(F)
  Fsqrt = np.linalg.cholesky(F)

  V = np.random.randn(m, n) / np.sqrt(m + n)

  Ddiag = 0.1 * np.diag(V.transpose().dot(F).dot(V))
  D = np.diag(Ddiag)

  mu = np.random.uniform(size=n) * 4 + rfr - 2

  fdata = np.random.normal(size=(m, p))
  fdata = Fsqrt.dot(fdata)
  epsdata = np.random.normal(size=(n, p))
  epsdata = np.sqrt(Ddiag).reshape((n, 1)) * epsdata

  rdata = mu.reshape((n, 1)) + V.transpose().dot(fdata) + epsdata

  # linear regression object
  LinR = linear_model.LinearRegression()

  LinR.fit(fdata.transpose(), rdata.transpose())
  V0 = LinR.coef_.transpose()
  mu0 = LinR.intercept_

  B = fdata
  A = np.ones((p, m + 1))
  A[:, 1:] = B.transpose()
  Ainv = np.linalg.inv(A.transpose().dot(A))
  G = B.dot(B.transpose()) - B.dot(np.ones((p, p))).dot(B.transpose()) / p

  s2 = (np.sum((LinR.predict(fdata.transpose()) - rdata.transpose())
               ** 2, axis=0) / (p - m - 1)).transpose()
  c1sig = scipy.stats.f.cdf(sig, dfn=1, dfd=p - m - 1, loc=0, scale=1)
  cmsig = scipy.stats.f.cdf(sig, dfn=m, dfd=p - m - 1, loc=0, scale=1)
  gam = np.sqrt(Ainv[0, 0] * c1sig * s2)
  rho = np.sqrt(m * cmsig * s2)

  if(robust_return):
    modname = filename + '_robustReturn'
  else:
    modname = filename
  mod = gurobipy.Model(modname)
  mod.setParam('OutputFlag', 0)

  x = pd.Series(mod.addVars(range(n), name='asset'), index=range(n))
  a = mod.addVar(name='return_var', obj=-lamb)
  b = mod.addVar(name='uncertain_risk_var', obj=1)
  c = mod.addVar(name='fixed_risk_var', obj=1)

  mod.addConstr(x.sum() == 1, 'budget')
  mod.addQConstr(x.transpose().dot(V0.transpose()).dot(F).dot(V0).dot(x),
                 gurobipy.GRB.LESS_EQUAL,
                 b,
                 'uncertain_risk')
  mod.addQConstr(x.transpose().dot(D).dot(x),
                 gurobipy.GRB.LESS_EQUAL,
                 c,
                 'fixed_risk')

  if(robust_return):
    w = pd.Series(mod.addVars(range(n), name='abs_asset_var'), index=range(n))
    mod.addConstr(mu0.dot(x) >= a + gam.dot(w), 'robustReturn')
    mod.addConstrs((w[i] >= x[i] for i in range(n)), 'abs_asset1')
    mod.addConstrs((w[i] >= -x[i] for i in range(n)), 'abs_asset2')
    mod.addConstr(mu0.dot(x) >= a, 'return')
  else:
    mod.addConstr(mu0.dot(x) >= a, 'return')

  mod.update()
  mod.write(savedir + modname + '.mps')

  robustData = []
  constrData = dict()
  constrData['name'] = 'return'
  constrData['type'] = 'linear'
  constrData['nominal_coeff'] = dict()
  constrData['uncertainty'] = dict()
  con = mod.getConstrByName('return')
  constrData['id'] = con._rowno
  constrData['sense'] = con.sense
  constrData['RHS'] = con.RHS
  con_expr = mod.getRow(con)
  constrData['uncertainty']['dim'] = con_expr.size() - 1
  constrData['uncertainty']['type'] = 'supball'
  constrData['uncertainty']['radius'] = 1
  constrData['uncertainty']['data'] = dict()
  for i in range(con_expr.size() - 1):
    var = con_expr.getVar(i)
    varname = var.VarName
    varid = int(var._colno)
    coeff = con_expr.getCoeff(i)
    constrData['nominal_coeff'][varid] = [coeff, varname]
    constrData['uncertainty']['data'][varid] = [gam[i], varname]
  constrData['certain_variable_coefficient'] = [-1]
  constrData['certain_variable_name'] = ['return_var']
  robustData.append(constrData)

  k = 2 * m
  constrData = dict()
  constrData['name'] = 'uncertain_risk'
  for constr in mod.getQConstrs():
    if(constr.QCName == constrData['name']):
      con = constr
  constrData['id'] = con._qconstrno

  constrData['type'] = 'quadratic'

  constrData['vars'] = [int(v._colno) for v in x]

  qV0 = Fsqrt.transpose().dot(V0)
  constrData['base_matrix'] = dict()
  constrData['base_matrix']['nrows'] = qV0.shape[0]
  constrData['base_matrix']['ncols'] = qV0.shape[1]
  constrData['base_matrix']['format'] = 'dense'
  constrData['base_matrix']['vals'] = qV0

  constrData['uncertainty'] = dict()
  constrData['uncertainty']['type'] = 'L2ball'
  constrData['uncertainty']['radius'] = 1
  # of the format [(k,val) for k in K]
  #[] means the centre is the zero vector
  constrData['uncertainty']['centre'] = []
  constrData['uncertainty']['data'] = []

  for j in range(k):
    P = np.random.randn(m, n)
    Pnorm = np.sqrt(np.diag(P.transpose().dot(G).dot(P)))
    P = (P / Pnorm) * rho / np.sqrt(n)
    P = Fsqrt.transpose().dot(P)
    P = np.around(P, decimals=9)
    matrix = dict()
    matrix['nrows'] = P.shape[0]
    matrix['ncols'] = P.shape[1]
    matrix['format'] = 'dense'
    matrix['vals'] = P
    constrData['uncertainty']['data'].append(matrix)
  constrData['certain_variable_coefficient'] = [-1]
  constrData['certain_variable_name'] = ['uncertain_risk_var']
  robustData.append(constrData)

  with open(savedir + modname + '.json', 'w') as f:
    json.dump(robustData, f, cls=MyEncoder, indent=4)



# np.random.seed(1)
# number of assets
n = 10
# number of factors
m = 4
# risk-free rate
rfr = 3
# number of samples
p = 90
# significance level for uncertainty sets
sig = 0.95
# balance between maximizing return and minimizing risk
lamb = 1
# robust return constraint flag (False: only include the nominal constraint. True: includ ethe robust constraint also)
robust_return = False

robustPort(filename=None, savedir='../instances',  # '',#
           n=10, m=4, rfr=3, p=90,
           lamb=1, sig=0.99,
           robust_return=False)
