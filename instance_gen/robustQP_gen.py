# -*- coding: utf-8 -*-
"""
Created on Fri Apr 28 10:26:33 2017

@author: hnh
"""

from gurobipy import *
import os
import numpy as np
import pandas as pd
from copy import deepcopy
import json

savedir = '../instances/'

def namer(newname='instQP_0', savedir='../instances'):
    savedir += '/'
    if not os.path.exists(savedir):
        names = set()
    else:
        names = os.listdir(savedir)
        names = [name.strip('.mps') for name in names if name.split('.')[-1]=='mps']
    suffix = newname.split('_')[-1]
    try:
        suffix = str(int(suffix)+1)
        newname = '_'.join(newname.split('_')[:-1]) + '_' + suffix
    except:
        newname += '_0'
    while newname in names:
        suffix = newname.split('_')[-1]
        try:
            suffix = str(int(suffix)+1)
            newname = '_'.join(newname.split('_')[:-1]) + '_' + suffix
        except:
            newname += '_0'
    
    return newname

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


def robustQP(filename=None, savedir='../instances',
             n=10, m=2, Kmax=4,
             variation=0.1,
             round_digits=3):
    """Inputs:
        filename: name of new instance to generate
        savedir: directory to store new instance
        n: dimension of nominal model
        m: number of (quadratic) constraints
        Kmax: maximum dimension of each constraint uncertainty set
              (actual dimension is randomly generated for each constraint)
        variation: level of deviation of robustness data from nominal data
        round_digits: number of digits to round to.
    """
    
    if(filename==None):
        filename = 'instQP_n%i_m%i' %(n,m)
        filename = namer(filename, savedir)
    savedir += '/'
    if not os.path.exists(savedir):
        os.makedirs(savedir)
    
    #WLOG assume the objective is full rank and has identity quadratic part
    #A0 = np.eye(n)
    b0 = np.random.randn(n)
    b0 = np.round(b0, round_digits)
    c0 = np.random.randn()
    c0 = np.round(c0, round_digits)
    
    A = dict()
    b = dict()
    c = dict()
    P = dict()
    robustData = []
    for i in range(m):
        #choose how many vars
        nv = np.random.randint(1, n)
        #choose which vars
        v = np.random.choice(range(n), nv, replace=False)
        v.sort()
        ni = np.random.randint(int(n/4), n)
        A[i] = np.zeros((ni,n))
        Amat = np.random.randn(ni,nv)
        A[i][:,v] = deepcopy(Amat)
        A[i] = np.round(A[i], round_digits)
        
        b[i] = np.random.randn(n)
        b[i][v] = 0
        b[i] = np.round(b[i], round_digits)
        
        c[i] = np.random.exponential(4)
        c[i] = np.round(c[i], round_digits)
        
        constrData = dict()
        
        name = 'quad_%i' %i
        constrData['name'] = name
        constrData['type'] = 'quadratic'
        
        constrData['vars'] = [j for j in range(n)]
        
        constrData['uncertainty'] = dict()
        constrData['uncertainty']['type'] = 'L2ball'
        
        K = np.random.randint(0,Kmax)
        if(K <= 1):
            continue
        constrData['uncertainty']['dim'] = K
        
        radius = 1
        constrData['uncertainty']['radius'] = radius
        
        #of the format [(k,val) for k in K]
        #[] means the centre is the zero vector
        constrData['uncertainty']['centre'] = []
        
        constrData['uncertainty']['data'] = dict()
        
        nrows = A[i].shape[0]
        constrData['uncertainty']['data']['nrows'] = nrows
        ncols = A[i].shape[1]
        constrData['uncertainty']['data']['ncols'] = ncols
        
        baseMatrix = dict()
        baseMatrix['format'] = 'sparse_coo'
        baseMatrix['nrows'] = nrows
        baseMatrix['ncols'] = ncols
        baseMatrix['nnz'] = len(v)*n
        baseMatrix['vals'] = [(ix,iy,A[i][ix,iy]) for ix in range(ni) for iy in v]
        constrData['base_matrix'] = baseMatrix
        
        constrData['uncertainty']['data']['matrices'] = []
        
        pairlist = [(j,l) for j in v for l in range(n)]
        num_uncertain = np.random.randint(0,len(pairlist))
        uncertain_idx = np.random.choice(range(len(pairlist)), num_uncertain, replace=False)
        uncertain_idx.sort()
        uncertain_pairs = [pairlist[idx] for idx in uncertain_idx]
        for k in range(K):
            matrix = dict()
            matrix['format'] = 'sparse_coo'
            matrix['nrows'] = nrows
            matrix['ncols'] = ncols
            matrix['nnz'] = len(uncertain_pairs)
            randvals = np.random.rand(num_uncertain)*0.1-0.05
            randvals = np.round(randvals, round_digits)
            matrix['vals'] = [(j,l,randvals[p]) for p,(j,l) in enumerate(uncertain_pairs)]
            constrData['uncertainty']['data']['matrices'].append(matrix)
        
        robustData.append(constrData)
    
    with open(savedir + filename + '.json', 'w') as f:
        json.dump(robustData, f, cls=MyEncoder)
    
    
    mod = Model('nominalQP')
    mod.setParam('OutputFlag', 0)
    
    x = pd.Series(mod.addVars(range(n)), index=range(n))
    
    #note: A0 is always identity
    quad_obj = x.dot(x) + b0.dot(x) + c0
    mod.setObjective(quad_obj, GRB.MINIMIZE)
    
    
    for i in range(m):
        Ai = A[i].transpose().dot(A[i])
        bi = b[i]
        ci = c[i]
        qc = mod.addQConstr(x.dot(Ai).dot(x) + bi.dot(x),
                            GRB.LESS_EQUAL,
                            ci,
                            'quad_%i' %i)
    
    mod.update()
    
    mod.write(savedir + filename + '.mps')




n = 20
m = 4
Kmax = 7
round_digits = 3
robustQP(filename=None,
         savedir='../instances',
         n=n, m=m, Kmax=Kmax, round_digits=roung_digits)


















#def robustQP_old(filename=None, savedir='../instances', n=10, m=2, Kmax=4, round_digits=3):
#    
#    if(filename==None):
#        filename = 'instQP_n%i_m%i' %(n,m)
#        filename = namer(filename, savedir)
#    savedir += '/'
#    if not os.path.exists(savedir):
#        os.makedirs(savedir)
#    
#    
#    #WLOG assume the objective is full rank and has identity quadratic part
#    #A0 = np.eye(n)
#    b0 = np.random.randn(n)
#    b0 = np.round(b0, round_digits)
#    c0 = np.random.randn()
#    c0 = np.round(c0, round_digits)
#    
#    A = dict()
#    b = dict()
#    c = dict()
#    P = dict()
#    S = dict()
#    for i in range(m):
#        #choose how many vars
#        nv = np.random.randint(1, n)
#        #choose which vars
#        v = np.random.choice(range(n), nv, replace=False)
#        v.sort()
#        ni = np.random.randint(int(n/4), n)
#        A[i] = np.zeros((ni,n))
#        Amat = np.random.randn(ni,nv)
#        A[i][:,v] = deepcopy(Amat)
#        A[i] = np.round(A[i], round_digits)
#        S[i] = A[i].shape
#        
#        b[i] = np.random.randn(n)
#        b[i][v] = 0
#        b[i] = np.round(b[i], round_digits)
#        
#        c[i] = np.random.exponential(4)
#        c[i] = np.round(c[i], round_digits)
#        
#        K = np.random.randint(0,Kmax)
#        if(K <= 1):
#            continue
#        pairlist = [(j,l) for j in v for l in range(n)]
#        num_uncertain = np.random.randint(0,len(pairlist))
#        uncertain_idx = np.random.choice(range(len(pairlist)), num_uncertain, replace=False)
#        uncertain_idx.sort()
#        uncertain_pairs = [pairlist[idx] for idx in uncertain_idx]
#        for k in range(K):
#            randvals = np.random.rand(num_uncertain)*0.1-0.05
#            randvals = np.round(randvals, round_digits)
#            P['quad_%i' %i,k] = [(j,l,randvals[p]) for p,(j,l) in enumerate(uncertain_pairs)]
#    
#    mod = Model('nominalQP')
#    mod.setParam('OutputFlag', 0)
#    
#    x = pd.Series(mod.addVars(range(n)), index=range(n))
#    
#    #note: A0 is always identity
#    quad_obj = x.dot(x) + b0.dot(x) + c0
#    mod.setObjective(quad_obj, GRB.MINIMIZE)
#    
#    
#    for i in range(m):
#        Ai = A[i].transpose().dot(A[i])
#        bi = b[i]
#        ci = c[i]
#        qc = mod.addQConstr(x.dot(Ai).dot(x) + bi.dot(x),
#                            GRB.LESS_EQUAL,
#                            ci,
#                            'quad_%i' %i)
#    
#    mod.update()
#    
#    mod.write(savedir + filename + '.mps')
#    
#    with open(savedir + filename + '.csv','w') as f:
#        quadconstrs = mod.getQConstrs()
#        for con in quadconstrs:
#            s = ''
#            
#            name = con.getAttr('QCName')
#            s += name + ','
#            
#            constr_type = 'quadratic'
#            s += constr_type + ','
#            
#            uncertainty_type = 'L2ball'
#            s += uncertainty_type + ','
#            
#            Klist = [k for (cname,k) in P.keys() if cname==name]
#            Klist.sort()
#            dim = len(Klist)
#            s += str(dim) + ','
#            if dim==0:
#                continue
#            
#            radius = 1
#            s += str(radius) + ','
#            
#            nrows = S[i][0]
#            s += str(nrows) + ','
#            
#            ncols = S[i][1]
#            s += str(ncols) + ','
#            
#            nnz = sum([len(P[name,k]) for k in Klist])
#            s += str(nnz) + ','
#            
#            for k in Klist:
#                for tr in P[name,k]:
#                    newtr = [str(k),str(tr[0]),str(tr[1]),str(tr[2])]
#                    trstring = ' '.join(newtr)
#                    s += trstring + ','
#            
#            s += '\n'
#            
#            f.write(s)


