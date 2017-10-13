# -*- coding: utf-8 -*-
"""
Created on Thu Aug 10 21:16:13 2017

@author: hnh
"""

import pandas as pd
import numpy as np
from sklearn import linear_model
import scipy.stats
import gurobipy
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


df = pd.read_csv('SP500_data_returns.csv')
df2 = pd.read_csv('FF3factor.csv')

np.random.seed(0)


def sp500Gen(filename = None, period_length = 10, gap = 90, num_portfolios = 5, 
  num_stocks = 482, sig = 0.95, lamb = 1, 
  robust_return = False, 
  savedir = '../../instances/SP500_instances'):
  if not os.path.exists(savedir):
    os.mkdir(savedir)
  fullcodes = df.PERMNO.unique()
  assert(num_stocks <= len(fullcodes))
  codes = np.random.choice(fullcodes, size=num_stocks, replace=False)
  n = len(codes)
  m = 3
  p = period_length
  dates = df.date.unique()
  assert(num_portfolios * gap < len(dates) - period_length)
  lastdate = sorted(dates)[-1]
  startidx = 0
  reached_end = False
  count = 0
  while(not reached_end):
    datevec = dates[startidx:(startidx + p)]
    startdate = datevec[0]
    enddate = datevec[-1]
    returns = df[['date', 'PERMNO', 'return']].query(
        'date>=@startdate and date<=@enddate and PERMNO in @codes')
    returns = returns.pivot(index='date', columns='PERMNO', values='return')
    returns.fillna(0, inplace=True)
    factors = df2.query('date>=@startdate and date<=@enddate')
    factors.set_index('date', inplace=True)
    RFvals = np.repeat(factors.mkt.values, repeats=n).reshape(p, n)
    rf = pd.DataFrame(RFvals, columns=returns.columns, index=datevec)
    yvals = returns - rf
    labels = factors[['mktrf', 'smb', 'hml']]

    fdata = labels.values.T
    rdata = yvals.values.T
    # linear regression object
    LinR = linear_model.LinearRegression()

    LinR.fit(fdata.transpose(), rdata.transpose())
    V0 = LinR.coef_.transpose()
    mu0 = LinR.intercept_
    residuals = V0.T.dot(fdata) + mu0.reshape(n, 1) - rdata
    residuals = pd.DataFrame(residuals.T, columns=codes, index=datevec)
    D = np.diag(np.diag(residuals.cov().values))

    F = labels.cov().values
    Fsqrt = np.linalg.cholesky(F)
    eigF = np.linalg.eigvalsh(F)

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
    if filename == None:
      filename = 'SP500_n%i_%i_%i' % (n, startdate, enddate)
      if(robust_return):
        modname = filename + '_robustReturn'
      else:
        modname = filename
    else:
      modname = filename

    mod = gurobipy.Model(modname)
    mod.setParam('OutputFlag', 0)

    x = pd.Series(mod.addVars(codes, name='asset', lb=-
                              gurobipy.GRB.INFINITY))
    a = mod.addVar(name='return_var', obj=-lamb, lb=-gurobipy.GRB.INFINITY)
    b = mod.addVar(name='uncertain_risk_var', obj=1, lb=-gurobipy.GRB.INFINITY)
    c = mod.addVar(name='fixed_risk_var', obj=1, lb=-gurobipy.GRB.INFINITY)

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
      mod.addConstr(-mu0.dot(x) <= -a, 'return')
    else:
      mod.addConstr(-mu0.dot(x) <= -a, 'return')

    mod.update()
    mod.write(savedir + '/' + modname + '.mps')

    robustData = []
    if robust_return:
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

    constrData['RHS'] = 0
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
    constrData['certain_variable_index'] = [b._colno]
    robustData.append(constrData)

    with open(savedir + '/' + modname + '.json', 'w') as f:
      json.dump(robustData, f, cls=MyEncoder, indent=4)

    print(count)
    startidx += gap
    count += 1
    if(enddate == lastdate or count >= num_portfolios):
      reached_end = True
