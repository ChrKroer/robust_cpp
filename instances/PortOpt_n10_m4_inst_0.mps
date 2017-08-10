NAME PortOpt_n10_m4_inst_0
ROWS
 N  OBJ
 E  budget  
 L  return  
 L  uncertain_risk
 L  fixed_risk
COLUMNS
    asset[0]  budget    1
    asset[0]  return    -1.8946743379288904e+00
    asset[1]  budget    1
    asset[1]  return    -2.7300045346921520e+00
    asset[2]  budget    1
    asset[2]  return    -3.9292181176231424e+00
    asset[3]  budget    1
    asset[3]  return    -1.3527731556397544e+00
    asset[4]  budget    1
    asset[4]  return    -4.5412900565214027e+00
    asset[5]  budget    1
    asset[5]  return    -4.1573975389446449e+00
    asset[6]  budget    1
    asset[6]  return    -3.8829068291859903e+00
    asset[7]  budget    1
    asset[7]  return    -2.0892733582390948e+00
    asset[8]  budget    1
    asset[8]  return    -2.9517542308778584e+00
    asset[9]  budget    1
    asset[9]  return    -4.9910721563286788e+00
    return_var  OBJ       -1
    return_var  return    1
    uncertain_risk_var  OBJ       1
    uncertain_risk_var  uncertain_risk  -1
    fixed_risk_var  OBJ       1
    fixed_risk_var  fixed_risk  -1
RHS
    RHS1      budget    1
BOUNDS
 FR BND1      asset[0]
 FR BND1      asset[1]
 FR BND1      asset[2]
 FR BND1      asset[3]
 FR BND1      asset[4]
 FR BND1      asset[5]
 FR BND1      asset[6]
 FR BND1      asset[7]
 FR BND1      asset[8]
 FR BND1      asset[9]
 FR BND1      return_var
 FR BND1      uncertain_risk_var
 FR BND1      fixed_risk_var
QCMATRIX   uncertain_risk
    asset[0]  asset[0]  6.0631450131305858e-02
    asset[0]  asset[1]  6.0581595768836524e-02
    asset[1]  asset[0]  6.0581595768836524e-02
    asset[0]  asset[2]  -4.0599540846104634e-02
    asset[2]  asset[0]  -4.0599540846104634e-02
    asset[0]  asset[3]  -4.1271239298307179e-02
    asset[3]  asset[0]  -4.1271239298307179e-02
    asset[0]  asset[4]  2.1437442251282284e-02
    asset[4]  asset[0]  2.1437442251282284e-02
    asset[0]  asset[5]  4.7672768387795703e-02
    asset[5]  asset[0]  4.7672768387795703e-02
    asset[0]  asset[6]  -7.3352862636805613e-02
    asset[6]  asset[0]  -7.3352862636805613e-02
    asset[0]  asset[7]  5.1233996146163186e-03
    asset[7]  asset[0]  5.1233996146163186e-03
    asset[0]  asset[8]  -4.9223174483041837e-02
    asset[8]  asset[0]  -4.9223174483041837e-02
    asset[0]  asset[9]  8.8039936081670045e-02
    asset[9]  asset[0]  8.8039936081670045e-02
    asset[1]  asset[1]  1.4501398172692559e-01
    asset[1]  asset[2]  -4.6067211615133469e-02
    asset[2]  asset[1]  -4.6067211615133469e-02
    asset[1]  asset[3]  -1.4861948928563495e-01
    asset[3]  asset[1]  -1.4861948928563495e-01
    asset[1]  asset[4]  -3.4892435004890421e-02
    asset[4]  asset[1]  -3.4892435004890421e-02
    asset[1]  asset[5]  5.7966398296365226e-02
    asset[5]  asset[1]  5.7966398296365226e-02
    asset[1]  asset[6]  -1.1882941916186605e-01
    asset[6]  asset[1]  -1.1882941916186605e-01
    asset[1]  asset[7]  3.2786711661234286e-02
    asset[7]  asset[1]  3.2786711661234286e-02
    asset[1]  asset[8]  -8.5202288090357084e-02
    asset[8]  asset[1]  -8.5202288090357084e-02
    asset[1]  asset[9]  2.4518285819206687e-01
    asset[9]  asset[1]  2.4518285819206687e-01
    asset[2]  asset[2]  3.0838392381791757e-02
    asset[2]  asset[3]  3.4116913534443336e-02
    asset[3]  asset[2]  3.4116913534443336e-02
    asset[2]  asset[4]  -1.9908624940594526e-02
    asset[4]  asset[2]  -1.9908624940594526e-02
    asset[2]  asset[5]  -4.3301224562876647e-02
    asset[5]  asset[2]  -4.3301224562876647e-02
    asset[2]  asset[6]  5.5922553621119427e-02
    asset[6]  asset[2]  5.5922553621119427e-02
    asset[2]  asset[7]  -8.7402322181066236e-03
    asset[7]  asset[2]  -8.7402322181066236e-03
    asset[2]  asset[8]  4.5111315692855633e-02
    asset[8]  asset[2]  4.5111315692855633e-02
    asset[2]  asset[9]  -7.7538423013307073e-02
    asset[9]  asset[2]  -7.7538423013307073e-02
    asset[3]  asset[3]  1.6600735558983976e-01
    asset[3]  asset[4]  5.1332136584363516e-02
    asset[4]  asset[3]  5.1332136584363516e-02
    asset[3]  asset[5]  -5.2665062939957416e-02
    asset[5]  asset[3]  -5.2665062939957416e-02
    asset[3]  asset[6]  1.0671881591559965e-01
    asset[6]  asset[3]  1.0671881591559965e-01
    asset[3]  asset[7]  -4.1824394594205151e-02
    asset[7]  asset[3]  -4.1824394594205151e-02
    asset[3]  asset[8]  9.6501277651925815e-02
    asset[8]  asset[3]  9.6501277651925815e-02
    asset[3]  asset[9]  -2.7983219553824934e-01
    asset[9]  asset[3]  -2.7983219553824934e-01
    asset[4]  asset[4]  1.0819787426036728e-01
    asset[4]  asset[5]  8.5942644405154714e-02
    asset[5]  asset[4]  8.5942644405154714e-02
    asset[4]  asset[6]  -3.7163268357204215e-03
    asset[6]  asset[4]  -3.7163268357204215e-03
    asset[4]  asset[7]  1.2734863621836196e-02
    asset[7]  asset[4]  1.2734863621836196e-02
    asset[4]  asset[8]  -1.1943488591216003e-01
    asset[8]  asset[4]  -1.1943488591216003e-01
    asset[4]  asset[9]  6.2168220097333027e-02
    asset[9]  asset[4]  6.2168220097333027e-02
    asset[5]  asset[5]  1.3032967694175882e-01
    asset[5]  asset[6]  -7.2497888561431245e-02
    asset[6]  asset[5]  -7.2497888561431245e-02
    asset[5]  asset[7]  4.2939468944266534e-02
    asset[7]  asset[5]  4.2939468944266534e-02
    asset[5]  asset[8]  -1.9667135463436330e-01
    asset[8]  asset[5]  -1.9667135463436330e-01
    asset[5]  asset[9]  2.5440082650102930e-01
    asset[9]  asset[5]  2.5440082650102930e-01
    asset[6]  asset[6]  1.1794638666736523e-01
    asset[6]  asset[7]  -2.3829914760344153e-02
    asset[7]  asset[6]  -2.3829914760344153e-02
    asset[6]  asset[8]  8.3483215604431532e-02
    asset[8]  asset[6]  8.3483215604431532e-02
    asset[6]  asset[9]  -1.9310017085394537e-01
    asset[9]  asset[6]  -1.9310017085394537e-01
    asset[7]  asset[7]  2.3479634295718699e-02
    asset[7]  asset[8]  -7.8100016856825299e-02
    asset[8]  asset[7]  -7.8100016856825299e-02
    asset[7]  asset[9]  1.2679335624370541e-01
    asset[9]  asset[7]  1.2679335624370541e-01
    asset[8]  asset[8]  3.4529852054056476e-01
    asset[8]  asset[9]  -4.6083641487532778e-01
    asset[9]  asset[8]  -4.6083641487532778e-01
    asset[9]  asset[9]  7.8089440707319102e-01
QCMATRIX   fixed_risk
    asset[0]  asset[0]  6.1917792992913735e-03
    asset[1]  asset[1]  1.4081792582685075e-02
    asset[2]  asset[2]  2.9532690600636807e-03
    asset[3]  asset[3]  1.6729594555911654e-02
    asset[4]  asset[4]  1.2372269996085641e-02
    asset[5]  asset[5]  1.3465267445171965e-02
    asset[6]  asset[6]  1.1258167847384908e-02
    asset[7]  asset[7]  2.1687890346111312e-03
    asset[8]  asset[8]  3.4260608356908639e-02
    asset[9]  asset[9]  8.2551218316433805e-02
ENDATA
