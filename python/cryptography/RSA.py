from math import gcd, lcm
import secrets


class Prime:
    # Pre generated primes
    __first_primes_list = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                     31, 37, 41, 43, 47, 53, 59, 61, 67,
                     71, 73, 79, 83, 89, 97, 101, 103,
                     107, 109, 113, 127, 131, 137, 139,
                     149, 151, 157, 163, 167, 173, 179,
                     181, 191, 193, 197, 199, 211, 223,
                     227, 229, 233, 239, 241, 251, 257,
                     263, 269, 271, 277, 281, 283, 293,
                     307, 311, 313, 317, 331, 337, 347, 349]
    @staticmethod
    def __n_bit_random(n: int):
        _min = pow(2, n - 1) + 1
        _max = pow(2, n) - 1
        return _min + secrets.randbelow(_max - _min)


    @staticmethod
    def __get_low_level_prime(n):

        while True:
            # Obtain a random number
            pc = Prime.__n_bit_random(n)
    
            # Test divisibility by pre-generated
            # primes
            for divisor in Prime.__first_primes_list:
                if pc % divisor == 0 and divisor**2 <= pc:
                    break
            else:
                return pc
 
    def __is_miller_rabin_passed(mrc):
        '''Run 20 iterations of Rabin Miller Primality test'''
        maxDivisionsByTwo = 0
        ec = mrc-1
        while ec % 2 == 0:
            ec >>= 1
            maxDivisionsByTwo += 1
        assert(2**maxDivisionsByTwo * ec == mrc-1)
    
        def trialComposite(round_tester):
            if pow(round_tester, ec, mrc) == 1:
                return False
            for i in range(maxDivisionsByTwo):
                if pow(round_tester, 2**i * ec, mrc) == mrc-1:
                    return False
            return True
    
        # Set number of trials here
        numberOfRabinTrials = 20
        for _ in range(numberOfRabinTrials):
            round_tester = 2 + secrets.randbelow(mrc - 2)
            if trialComposite(round_tester):
                return False
        return True

    @staticmethod
    def gen_prime(bits: int) -> int:
        while True:
            candidate = Prime.__get_low_level_prime(bits)
            if Prime.__is_miller_rabin_passed(candidate):
                return candidate
            

class RSA:

    @staticmethod
    def __encode(text: str) -> int:
        assert len(text) < 2048
        res = 0
        for i in range(len(text) - 1, -1, -1):
            res = res * 256 + ord(text[i])
        return res

    @staticmethod
    def __decode(num: int) -> str:
        res = ''
        while num > 0:
            res += chr(num % 256)
            num //= 256
        return res
    
    @staticmethod
    def gen_keys() -> tuple[tuple[int, int], tuple[int, int]]:    
        p, q = Prime.gen_prime(512), Prime.gen_prime(512)

        N = p * q
        lambda_N = lcm(p - 1, q - 1)

        e = 65537
        d = pow(e, -1, lambda_N)

        return ((e, N), (d, N))

    @staticmethod
    def encrypt(message: str, public: tuple[int, int]):
        e, N = public
        m = RSA.__encode(message)
        assert m < N, "message is too big"
        
        c = pow(m, e, N)
        return RSA.__decode(c)

    @staticmethod
    def decrypt(ciphertext: str, private: tuple[int, int]):
        d, N = private
        c = RSA.__encode(ciphertext)
        assert c < N, "ciphertext is too big"

        m = pow(c, d, N)

        return RSA.__decode(m)