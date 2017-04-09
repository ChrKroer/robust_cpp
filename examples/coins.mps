NAME 
* Max problem is converted into Min one
ROWS
 N  OBJ
 E  Copper  
 E  Nickel  
 E  Zinc    
 E  Manganese
COLUMNS
    Pennies   OBJ       -0.01
    Pennies   Copper    0.06
    Pennies   Zinc      2.4
    Nickels   OBJ       -0.05
    Nickels   Copper    3.8
    Nickels   Nickel    1.2
    Dimes     OBJ       -0.1
    Dimes     Copper    2.1
    Dimes     Nickel    0.2
    Quarters  OBJ       -0.25
    Quarters  Copper    5.2
    Quarters  Nickel    0.5
    Dollars   OBJ       -1
    Dollars   Copper    7.2
    Dollars   Nickel    0.2
    Dollars   Zinc      0.5
    Dollars   Manganese  0.3
    Cu        Copper    -1
    Ni        Nickel    -1
    Zi        Zinc      -1
    Mn        Manganese  -1
RHS
BOUNDS
 UP BND1      Cu        1000
 UP BND1      Ni        50
 UP BND1      Zi        50
 UP BND1      Mn        50
ENDATA
