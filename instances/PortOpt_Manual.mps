NAME PortOpt_Manual
ROWS
 N  OBJ
 E  budget  
 G  return  
 L  uncertain_risk
 L  fixed_risk
COLUMNS
    asset[0]  budget    1
    asset[0]  return    1.1610888476672399e+00
    return_var  OBJ       -1
    return_var  return    -1
    uncertain_risk_var  OBJ       1
    uncertain_risk_var  uncertain_risk  -1
    fixed_risk_var  OBJ       1
    fixed_risk_var  fixed_risk  -1
RHS
    RHS1      budget    1
BOUNDS
QCMATRIX   uncertain_risk
    asset[0]  asset[0]  1.000e-02
QCMATRIX   fixed_risk
    asset[0]  asset[0]  1.000e+00
ENDATA
