import random
import string

def generate_random_string(length):
    # Define the characters you want to include in the random string
    characters = string.ascii_letters[26:]  # You can customize this based on your needs

    # Generate the random string
    random_string = ''.join(random.choice(characters) for _ in range(length))

    return random_string


phonebook_to_generate = 5
entry_per_phonebook = 100000

for p in range (0,phonebook_to_generate):
    # Clear File
    with open(f"p{p+1}.txt","w") as phonebook:
        phonebook.write('')
    phonebook.close()

    # Phonebook Entry
    with open(f"p{p+1}.txt","a") as phonebook:
        for contact in range(0,entry_per_phonebook):
            phonebook.write(f"{generate_random_string(random.randint(4,6))} {random.randint(1111111111,1999999999)}")
            phonebook.write('\n')


    phonebook.close()