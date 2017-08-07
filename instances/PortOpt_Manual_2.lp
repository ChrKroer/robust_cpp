\ Model PortOpt_Manual
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  - return_var + uncertain_risk_var + fixed_risk_var
Subject To
 budget: asset[0] = 1
 return: 1.16108884766724 asset[0] - return_var >= 0
 uncertain_risk: - uncertain_risk_var + [ 0.01 asset[0] ^2 ] <= 0
 fixed_risk: - fixed_risk_var + [ asset[0] ^2 ] <= 0
Bounds
End
