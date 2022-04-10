import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    database_file_path = sys.argv[1]
    sequence_file_path = sys.argv[2]

    # TODO: Read database file into a variable
    database = list()
    subsequences = list()

    with open(database_file_path, "r") as file:
        reader = csv.DictReader(file)
        subsequences = reader.fieldnames[1:]

        for row in reader:
            for subsequence in subsequences:
                row[subsequence] = int(row[subsequence])
            database.append(row)

    # TODO: Read DNA sequence file into a variable
    sequence = str()
    with open(sequence_file_path, "r") as file:
        reader = csv.reader(file)
        for row in reader:
            sequence += row[0]

    result = dict()
    # TODO: Find longest match of each STR in DNA sequence
    for subsequence in subsequences:
        count = longest_match(sequence, subsequence)
        result[subsequence] = count

    match = "No match"
    # TODO: Check database for matching profiles
    for person in database:
        if match_person(person, result, subsequences):
            match = person["name"]
            break

    print(match)
    return


def match_person(person_a, person_b, subsequences):
    for subsequence in subsequences:
        if person_a[subsequence] != person_b[subsequence]:
            return False
    return True


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
