from math import gcd 

def gen_keys() -> tuple[tuple[int, int], tuple[int, int]]:    
    p, q = 12345678901234567890123456, 123456789012345678901234

    N = p * q
    