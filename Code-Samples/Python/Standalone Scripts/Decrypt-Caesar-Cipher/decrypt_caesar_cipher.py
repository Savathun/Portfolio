
def decrypt(shift, ciphertext):
    """ Decrypt message contained in ciphertext using a right shift of shift. """
    decrypted_message = ""
    for char in ciphertext.lower():
        if char != ' ':
            decrypted_message += chr((ord(char) + shift - 97) % 26 + 97)
        else:
            decrypted_message += char
    return decrypted_message


def main():
    ciphertext = input("Enter ciphertext:  ").lower()
    # find the most common letters in the string
    from collections import Counter
    letter_frequencies = list(Counter(ciphertext).keys())
    # this string "etaoin..." represents the most common letters in english, in order.
    for letter in list('etaoinshrdlcumwfgypbvkjxqz'):
        # In order of most common letters in english, check if the most common letter in the string decrypts to one
        if input("Is it \"{}\"? [y/n]: ".format(decrypt(ord(letter) - ord(letter_frequencies[0]), ciphertext))) is 'y':
            break


if __name__ == "__main__":
    main()
