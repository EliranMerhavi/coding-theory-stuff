from RSA import RSA, Prime

def RSA_tests():
    #text = "Lorem ipsum dolor sit amet, consectetur adipisicing elit. Pariatur, eius nemo sapiente iste nisi illum enim totam deleniti odit est numquam ipsa fugiat, nulla perspiciatis nobis cumque beatae consectetur unde."
    text = "hello world potato"


    (private, public) = RSA.gen_keys()

    ciphertext = RSA.encrypt(text, public)
    plaintext = RSA.decrypt(ciphertext, private)
    assert plaintext == text
    print(plaintext)

def main():

    RSA_tests()
    

if __name__ == "__main__":
    main()