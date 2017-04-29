NAME nominalQP
ROWS
 N  OBJ
 L  quad_0  
 L  quad_1  
 L  quad_2  
 L  quad_3  
COLUMNS
    C0        OBJ       0.701
    C0        quad_0    0.227
    C0        quad_1    1.597
    C0        quad_2    0.896
    C1        OBJ       0.726
    C1        quad_0    -0.072
    C1        quad_1    -0.648
    C1        quad_2    0.514
    C2        OBJ       0.928
    C2        quad_0    0.145
    C2        quad_1    -0.96
    C2        quad_2    -2.277
    C3        OBJ       -0.608
    C3        quad_0    0.253
    C3        quad_1    1.331
    C3        quad_3    -0.157
    C4        OBJ       -2.557
    C4        quad_0    -1.602
    C4        quad_1    -1.311
    C4        quad_2    0.634
    C5        OBJ       1.825
    C5        quad_0    1.208
    C5        quad_2    0.51
    C6        OBJ       -2.133
    C6        quad_0    -2.13
    C6        quad_2    1.407
    C7        OBJ       -0.342
    C7        quad_0    -0.381
    C7        quad_1    -1.227
    C7        quad_2    -0.432
    C7        quad_3    -1.271
    C8        OBJ       -1.853
    C8        quad_1    -0.368
    C8        quad_3    0.809
    C9        OBJ       0.034
    C9        quad_0    -1.874
    C9        quad_1    -0.771
    C9        quad_2    -1.18
    C10       OBJ       -0.641
    C10       quad_0    1.956
    C10       quad_1    -0.331
    C10       quad_2    -1.183
    C11       OBJ       1.251
    C11       quad_0    1.433
    C11       quad_2    0.396
    C12       OBJ       -0.762
    C12       quad_0    1.195
    C12       quad_1    -1.295
    C12       quad_2    -0.667
    C13       OBJ       0.07
    C13       quad_0    -1.138
    C13       quad_1    -1.482
    C13       quad_3    -1.423
    C14       OBJ       1.906
    C14       quad_0    1.409
    C14       quad_1    -2.539
    C15       OBJ       0.892
    C15       quad_0    1.009
    C15       quad_1    -0.161
    C15       quad_2    -0.047
    C15       quad_3    -0.375
    C16       OBJ       1.055
    C16       quad_0    -0.072
    C16       quad_1    -0.45
    C16       quad_2    1.909
    C16       quad_3    1.468
    C17       OBJ       -0.362
    C17       quad_0    1.673
    C17       quad_1    -0.373
    C17       quad_2    -1.224
    C18       OBJ       0.718
    C18       quad_1    -0.802
    C18       quad_2    1.025
    C19       OBJ       0.187
    C19       quad_0    -0.225
    C19       quad_1    -0.453
    C19       quad_2    -0.272
RHS
    RHS1      OBJ       -1.649
    RHS1      quad_0    0.962
    RHS1      quad_1    4.112
    RHS1      quad_2    3.856
    RHS1      quad_3    12.685
BOUNDS
QUADOBJ
    C0        C0        2
    C1        C1        2
    C2        C2        2
    C3        C3        2
    C4        C4        2
    C5        C5        2
    C6        C6        2
    C7        C7        2
    C8        C8        2
    C9        C9        2
    C10       C10       2
    C11       C11       2
    C12       C12       2
    C13       C13       2
    C14       C14       2
    C15       C15       2
    C16       C16       2
    C17       C17       2
    C18       C18       2
    C19       C19       2
QCMATRIX   quad_0  
    C8        C8        1.8327869000000000e+001
    C8        C18       6.4777749999999994e+000
    C18       C8        6.4777749999999994e+000
    C18       C18       1.2882083000000000e+001
QCMATRIX   quad_1  
    C5        C5        1.0023472000000002e+001
    C5        C6        2.7431549999999998e+000
    C6        C5        2.7431549999999998e+000
    C5        C11       1.3697790000000001e+000
    C11       C5        1.3697790000000001e+000
    C6        C6        6.2047479999999995e+000
    C6        C11       -1.1494039999999999e+000
    C11       C6        -1.1494039999999999e+000
    C11       C11       6.0670269999999986e+000
QCMATRIX   quad_2  
    C3        C3        1.2329566000000000e+001
    C3        C8        6.9393399999999994e-001
    C8        C3        6.9393399999999994e-001
    C3        C13       4.2129299999999958e-001
    C13       C3        4.2129299999999958e-001
    C3        C14       -5.8341830000000003e+000
    C14       C3        -5.8341830000000003e+000
    C8        C8        1.9347477999999999e+001
    C8        C13       -7.2074430000000005e+000
    C13       C8        -7.2074430000000005e+000
    C8        C14       -3.6004239999999994e+000
    C14       C8        -3.6004239999999994e+000
    C13       C13       1.2088683000000001e+001
    C13       C14       -3.0872269999999999e+000
    C14       C13       -3.0872269999999999e+000
    C14       C14       1.1886139999999999e+001
QCMATRIX   quad_3  
    C0        C0        4.6740699999999995e+000
    C0        C1        -1.9582789999999994e+000
    C1        C0        -1.9582789999999994e+000
    C0        C2        1.7045500000000019e-001
    C2        C0        1.7045500000000019e-001
    C0        C4        -8.6513000000000104e-002
    C4        C0        -8.6513000000000104e-002
    C0        C5        5.5104200000000014e-001
    C5        C0        5.5104200000000014e-001
    C0        C6        2.4179339999999998e+000
    C6        C0        2.4179339999999998e+000
    C0        C9        -5.2711500000000011e-001
    C9        C0        -5.2711500000000011e-001
    C0        C10       -2.1736950000000004e+000
    C10       C0        -2.1736950000000004e+000
    C0        C11       -2.5983970000000003e+000
    C11       C0        -2.5983970000000003e+000
    C0        C12       2.2129759999999998e+000
    C12       C0        2.2129759999999998e+000
    C0        C14       -1.7548549999999998e+000
    C14       C0        -1.7548549999999998e+000
    C0        C17       2.0643989999999999e+000
    C17       C0        2.0643989999999999e+000
    C0        C18       -2.7127819999999998e+000
    C18       C0        -2.7127819999999998e+000
    C0        C19       4.1132920000000004e+000
    C19       C0        4.1132920000000004e+000
    C1        C1        5.4913609999999995e+000
    C1        C2        1.4695159999999998e+000
    C2        C1        1.4695159999999998e+000
    C1        C4        5.6094500000000014e-001
    C4        C1        5.6094500000000014e-001
    C1        C5        1.1517200000000003e+000
    C5        C1        1.1517200000000003e+000
    C1        C6        -2.0631599999999994e-001
    C6        C1        -2.0631599999999994e-001
    C1        C9        1.5254700000000010e-001
    C9        C1        1.5254700000000010e-001
    C1        C10       1.7666170000000001e+000
    C10       C1        1.7666170000000001e+000
    C1        C11       3.1651319999999994e+000
    C11       C1        3.1651319999999994e+000
    C1        C12       -1.2438470000000001e+000
    C12       C1        -1.2438470000000001e+000
    C1        C14       2.8008519999999999e+000
    C14       C1        2.8008519999999999e+000
    C1        C17       0.551186
    C17       C1        0.551186
    C1        C18       2.8508829999999996e+000
    C18       C1        2.8508829999999996e+000
    C1        C19       -6.9386279999999996e+000
    C19       C1        -6.9386279999999996e+000
    C2        C2        3.6830039999999999e+000
    C2        C4        -1.1256050000000000e+000
    C4        C2        -1.1256050000000000e+000
    C2        C5        2.3343889999999998e+000
    C5        C2        2.3343889999999998e+000
    C2        C6        1.9382799999999969e-001
    C6        C2        1.9382799999999969e-001
    C2        C9        -2.1783789999999996e+000
    C9        C2        -2.1783789999999996e+000
    C2        C10       5.2088799999999991e-001
    C10       C2        5.2088799999999991e-001
    C2        C11       -1.2818850000000004e+000
    C11       C2        -1.2818850000000004e+000
    C2        C12       3.11746
    C12       C2        3.11746
    C2        C14       -2.0214399999999996e-001
    C14       C2        -2.0214399999999996e-001
    C2        C17       -4.8000000000006886e-005
    C17       C2        -4.8000000000006886e-005
    C2        C18       -2.0933470000000001e+000
    C18       C2        -2.0933470000000001e+000
    C2        C19       -2.0347709999999997e+000
    C19       C2        -2.0347709999999997e+000
    C4        C4        1.3428720000000001e+000
    C4        C5        -2.0747350000000000e+000
    C5        C4        -2.0747350000000000e+000
    C4        C6        1.1955670000000000e+000
    C6        C4        1.1955670000000000e+000
    C4        C9        1.2055600000000009e-001
    C9        C4        1.2055600000000009e-001
    C4        C10       -1.0900109999999998e+000
    C10       C4        -1.0900109999999998e+000
    C4        C11       1.0188699999999994e-001
    C11       C4        1.0188699999999994e-001
    C4        C12       -1.4046590000000001e+000
    C12       C4        -1.4046590000000001e+000
    C4        C14       9.5102199999999981e-001
    C14       C4        9.5102199999999981e-001
    C4        C17       -6.7677999999999919e-002
    C17       C4        -6.7677999999999919e-002
    C4        C18       0.508106
    C18       C4        0.508106
    C4        C19       -1.5978240000000001e+000
    C19       C4        -1.5978240000000001e+000
    C5        C5        5.94533
    C5        C6        0.329953
    C6        C5        0.329953
    C5        C9        2.7120259999999998e+000
    C9        C5        2.7120259999999998e+000
    C5        C10       2.8018610000000002e+000
    C10       C5        2.8018610000000002e+000
    C5        C11       -6.6491199999999961e-001
    C11       C5        -6.6491199999999961e-001
    C5        C12       -4.3862900000000060e-001
    C12       C5        -4.3862900000000060e-001
    C5        C14       -1.9386149999999998e+000
    C14       C5        -1.9386149999999998e+000
    C5        C17       2.0336430000000001e+000
    C17       C5        2.0336430000000001e+000
    C5        C18       7.0702000000000009e-001
    C18       C5        7.0702000000000009e-001
    C5        C19       2.1082640000000001e+000
    C19       C5        2.1082640000000001e+000
    C6        C6        9.0036710000000024e+000
    C6        C9        3.25105
    C9        C6        3.25105
    C6        C10       -1.6696619999999998e+000
    C10       C6        -1.6696619999999998e+000
    C6        C11       -2.7470290000000004e+000
    C11       C6        -2.7470290000000004e+000
    C6        C12       6.7042400000000013e-001
    C12       C6        6.7042400000000013e-001
    C6        C14       -3.6935150000000001e+000
    C14       C6        -3.6935150000000001e+000
    C6        C17       2.88153
    C17       C6        2.88153
    C6        C18       -2.1021389999999993e+000
    C18       C6        -2.1021389999999993e+000
    C6        C19       1.1237020000000004e+000
    C19       C6        1.1237020000000004e+000
    C9        C9        7.4927939999999991e+000
    C9        C10       3.6679210000000002e+000
    C10       C9        3.6679210000000002e+000
    C9        C11       -1.7482299999999984e-001
    C11       C9        -1.7482299999999984e-001
    C9        C12       -3.9566719999999989e+000
    C12       C9        -3.9566719999999989e+000
    C9        C14       -2.888
    C14       C9        -2.888
    C9        C17       1.849
    C17       C9        1.849
    C9        C18       3.0878739999999998e+000
    C18       C9        3.0878739999999998e+000
    C9        C19       2.7410319999999997e+000
    C19       C9        2.7410319999999997e+000
    C10       C10       8.8860150000000004e+000
    C10       C11       2.6395810000000002e+000
    C11       C10       2.6395810000000002e+000
    C10       C12       3.3993829999999994e+000
    C12       C10       3.3993829999999994e+000
    C10       C14       -3.4983269999999997e+000
    C14       C10       -3.4983269999999997e+000
    C10       C17       3.2317700000000010e-001
    C17       C10       3.2317700000000010e-001
    C10       C18       1.5333620000000001e+000
    C18       C10       1.5333620000000001e+000
    C10       C19       -1.7557089999999997e+000
    C19       C10       -1.7557089999999997e+000
    C11       C11       1.0256702999999998e+001
    C11       C12       -1.4086799999999997e-001
    C12       C11       -1.4086799999999997e-001
    C11       C14       -0.702465
    C14       C11       -0.702465
    C11       C17       3.1202339999999995e+000
    C17       C11       3.1202339999999995e+000
    C11       C18       4.0631449999999996e+000
    C18       C11       4.0631449999999996e+000
    C11       C19       -3.6462080000000001e+000
    C19       C11       -3.6462080000000001e+000
    C12       C12       1.5563032999999999e+001
    C12       C14       -3.1821570000000001e+000
    C14       C12       -3.1821570000000001e+000
    C12       C17       -1.8693550000000001e+000
    C17       C12       -1.8693550000000001e+000
    C12       C18       -6.2369409999999998e+000
    C18       C12       -6.2369409999999998e+000
    C12       C19       -1.3511319999999998e+000
    C19       C12       -1.3511319999999998e+000
    C14       C14       8.9125820000000004e+000
    C14       C17       -4.6978770000000001e+000
    C17       C14       -4.6978770000000001e+000
    C14       C18       2.5037190000000007e+000
    C18       C14       2.5037190000000007e+000
    C14       C19       -5.3318250000000003e+000
    C19       C14       -5.3318250000000003e+000
    C17       C17       6.2689810000000001e+000
    C17       C18       5.3418200000000016e-001
    C18       C17       5.3418200000000016e-001
    C17       C19       2.4786549999999994e+000
    C19       C17       2.4786549999999994e+000
    C18       C18       5.5697679999999998e+000
    C18       C19       -2.1114359999999999e+000
    C19       C18       -2.1114359999999999e+000
    C19       C19       1.2547880000000001e+001
ENDATA
