import gurobipy

mod = gurobipy.Model('model1')
x = mod.addVar(name='x', lb=-gurobipy.GRB.INFINITY)
y = mod.addVar(name='y', lb=-gurobipy.GRB.INFINITY)
mod.addQConstr(x*x + 5*y*y, gurobipy.GRB.LESS_EQUAL, 1.0)
mod.addQConstr(5*x*x + y*y, gurobipy.GRB.LESS_EQUAL, 1.0)
mod.setObjective(x - y, gurobipy.GRB.MINIMIZE)
mod.update()
mod.optimize()
mod.write('trial_write.mps')