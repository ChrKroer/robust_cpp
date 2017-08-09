NAME model1
ROWS
 N  OBJ
 L  qc0     
 L  qc1     
COLUMNS
    asset[0]  OBJ       1
    asset[1]  OBJ       -1
RHS
    RHS1      qc0       1
    RHS1      qc1       1
BOUNDS
 FR BND1      asset[0]
 FR BND1      asset[1]
QCMATRIX   qc0     
    asset[0]  asset[0]  1
    asset[1]  asset[1]  4
QCMATRIX   qc1     
    asset[0]  asset[0]  4
    asset[1]  asset[1]  1
ENDATA
