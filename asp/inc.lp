#script (python)
from clingo import Function

def get(val, default):
    return val if val != None else default

def main(prg):
    imin  = get(prg.get_const("imin"), 1)
    imax  = prg.get_const("imax")
    istop = get(prg.get_const("istop"), "SAT")
    
    # parts = []
    # parts.append(("check", [0]))
    # parts.append(("base" , []))
    
    # prg.ground(parts)
    # prg.assign_external(Function("query", [0]), True)
    # ret = prg.solve()
    
    # parts = []
    # parts.append(("check", [1]))
    
    # prg.release_external(Function("query", [0]))
    # parts.append(("step", [1]))
    # prg.cleanup()
    
    # prg.ground(parts)
    # prg.assign_external(Function("query", [1]), True)
    
    # ret = prg.solve()
    
    # parts = []
    # parts.append(("check", [2]))
    
    # prg.release_external(Function("query", [1]))
    # parts.append(("step", [2]))
    # prg.cleanup()
    
    # prg.ground(parts)
    # prg.assign_external(Function("query", [2]), True)
    
    # ret = prg.solve()

    
    # parts = []
    # parts.append(("check", [3]))
    
    # prg.release_external(Function("query", [2]))
    # parts.append(("step", [3]))
    # prg.cleanup()
    
    # prg.ground(parts)
    # prg.assign_external(Function("query", [3]), True)
    
    # ret = prg.solve()

    step, ret = 0, None
    while ((imax is None or step < imax) and
            (step == 0 or step < imin or (
                (istop == "SAT"     and not ret.satisfiable  ) or
                (istop == "UNSAT"   and not ret.unsatisfiable) or
                (istop == "UNKNOWN" and not ret.unknown)))):
        parts = []
        parts.append(("check", [step]))
    
        if step > 0:
            prg.release_external(Function("query", [step-1]))
            parts.append(("step" , [step]))
            prg.cleanup()
        else:
            parts.append(("base", []))
    
        prg.ground(parts)
        prg.assign_external(Function("query", [step]), True)
        ret, step = prg.solve(), step+1
#end.

#program check(t).
#external query(t).
