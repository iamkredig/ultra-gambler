def get_quantum_numbers(requested_amount=1):
    collected_numbers = []
    print(f"\n[Python] Der Automat braucht {requested_amount} neue Zahl(en)!")

    for index in range(requested_amount):
        while True:
            try:
                user_input = input(f"Gib Zahl {index + 1} ein: ")
                valid_number = int(user_input)
                collected_numbers.append(valid_number)
                break
            except ValueError:
                print("Bitte gib eine gültige Ganzzahl ein!")

    # TEST-PRINT: Zeigt an, was Python jetzt zurück an C++ feuert
    print(f">>> [SUCCESS] Python leitet Zahl weiter: {collected_numbers} <<<\n")
    return collected_numbers
