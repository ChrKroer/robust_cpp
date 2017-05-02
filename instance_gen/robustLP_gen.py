# -*- coding: utf-8 -*-
"""
Created on Sat Apr 29 21:39:20 2017

@author: hnh
"""

import gurobipy
import os
import numpy as np
import json
from fractions import Fraction

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


def robustLP_from_MPS(infile,
                      filename=None,
                      savedir='../instances',
                      rad=1):
    """Inputs:
        infile: path for input MPS
        filename: name of new instance to generate
        savedir: directory to store new instance
        rad: radius of uncertainty set
    """
    
    infilename = os.path.basename(infile)
    infilename = infilename.strip('.mps')
    
    if(filename==None):
        filename = infilename + '_robust'
    if(len(savedir)>0):
        savedir += '/'
        if not os.path.exists(savedir):
            os.makedirs(savedir)
    
    
    mod = gurobipy.read(infile)
    mod.setParam('OutputFlag', 0)
    rad = 1
    
    robustData = []
    lin_constrs = mod.getConstrs()
    
    for con in lin_constrs:
        if(con.getAttr('sense')=='='):
            continue
        if(mod.getRow(con).size()<=1):
            continue
        con_expr = mod.getRow(con)
        
        constrData = dict()
            
        name = con.ConstrName
        conid = con._rowno
        constrData['name'] = name
        constrData['id'] = int(conid)
        
        constrData['type'] = 'linear'
        
        constrData['sense'] = con.sense
        constrData['RHS'] = con.RHS
        
        constrData['uncertainty'] = dict()
        constrData['nominal_coeff'] = dict()

        constrData['uncertainty']['radius'] = rad
        constrData['uncertainty']['data'] = dict()
        
        for i in range(con_expr.size()):
            var = con_expr.getVar(i)
            varname = var.VarName
            varid = int(var._colno)
            coeff = con_expr.getCoeff(i)
            frac = Fraction(str(coeff))
            if(frac.denominator<=100):
                constrData['nominal_coeff'][varid] = [coeff, varname]
            elif(frac.denominator%10==0 and frac.denominator <= 1000):
                constrData['nominal_coeff'][varid] = [coeff, varname]
            else:
                constrData['nominal_coeff'][varid] = [coeff, varname]
                constrData['uncertainty']['data'][varid] = [coeff, varname]
            
        if(len(constrData['uncertainty']['data'])==0):
            continue
        
        constrData['uncertainty']['dim'] = len(constrData['uncertainty']['data'])
        
        robustData.append(constrData)
        
    if(len(robustData)>0):
        with open(savedir + filename + '.json', 'w') as f:
            json.dump(robustData, f, cls=MyEncoder)




robustLP_from_MPS(infile='../instances/agg3.mps',
                  filename=None,#'test2',
                  #savedir='',
                  savedir='../instances/',
                  rad=1)


#mod = gurobipy.read('../instances/agg.mps')
#mod.setParam('OutputFlag', 0)
#mod.optimize()
#
#var = mod.getVars()
#d = {v._colno:v.VarName for v in var}

        
        












