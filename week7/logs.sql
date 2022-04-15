-- Keep a log of any SQL queries you execute as you solve the mystery.

-- more info about the crime:
SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- look interviews
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;
-- 161: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 162: Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- 163: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- 193: I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.

-- based on interview 161
SELECT BL.minute, BL.license_plate, P.name, P.phone_number, P.passport_number
FROM bakery_security_logs BL
JOIN people P ON BL.license_plate = P.license_plate
WHERE BL.year = 2021 AND BL.month = 7 AND BL.day = 28 AND BL.hour = 10 AND BL.minute > 15 AND BL.activity = 'exit';
-- | 16     | 5P2BI95       | Vanessa | (725) 555-4692 | 2963008352      |X
-- | 18     | 94KL13X       | Bruce   | (367) 555-5533 | 5773159633      |
-- | 18     | 6P58WS2       | Barry   | (301) 555-4174 | 7526138472      |X
-- | 19     | 4328GD8       | Luca    | (389) 555-5198 | 8496433585      |X
-- | 20     | G412CB7       | Sofia   | (130) 555-0289 | 1695452385      |X
-- | 21     | L93JTIZ       | Iman    | (829) 555-5269 | 7049073643      |X
-- | 23     | 322W7JE       | Diana   | (770) 555-1861 | 3592750733      |
-- | 23     | 0NTHK55       | Kelsey  | (499) 555-9472 | 8294398571      |X
-- | 35     | 1106N58       | Taylor  | (286) 555-6063 | 1988161715      |X

-- based on interview 162
SELECT ATM.amount, ATM.account_number, P.name, P.phone_number, P.passport_number, P.license_plate
FROM atm_transactions ATM
JOIN bank_accounts B ON ATM.account_number = B.account_number
JOIN people P ON P.id = B.person_id
WHERE ATM.year = 2021 AND ATM.month = 7 AND ATM.day = 28 AND ATM.atm_location = 'Leggett Street' AND ATM.transaction_type = 'withdraw';
-- | 50     | 49610011       | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 35     | 26013199       | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 80     | 16153065       | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |X
-- | 20     | 28296815       | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 20     | 25506511       | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |X
-- | 48     | 28500762       | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |X
-- | 60     | 76054385       | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |X
-- | 30     | 81061156       | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |

-- based on interview 163
SELECT PH.caller, PH.receiver, PC.id AS caller_id, PC.name AS caller_name, PR.id AS receiver_id, PR.name AS receiver_name
FROM phone_calls PH
JOIN people PC ON PC.phone_number = PH.caller
JOIN people PR ON PR.phone_number = PH.receiver
WHERE PH.year = 2021 AND PH.month = 7 AND PH.day = 28 AND PH.duration < 60;
-- | (130) 555-0289 | (996) 555-8899 | 398010    | Sofia       | 567218      | Jack          |X
-- | (499) 555-9472 | (892) 555-8872 | 560886    | Kelsey      | 251693      | Larry         |X
-- | (367) 555-5533 | (375) 555-8161 | 686048    | Bruce       | 864400      | Robin         |
-- | (499) 555-9472 | (717) 555-1342 | 560886    | Kelsey      | 626361      | Melissa       |X
-- | (286) 555-6063 | (676) 555-6554 | 449774    | Taylor      | 250277      | James         |X
-- | (770) 555-1861 | (725) 555-3243 | 514354    | Diana       | 847116      | Philip        |
-- | (031) 555-6622 | (910) 555-3251 | 907148    | Carina      | 712712      | Jacqueline    |X
-- | (826) 555-1652 | (066) 555-9701 | 395717    | Kenny       | 953679      | Doris         |
-- | (338) 555-6650 | (704) 555-2131 | 438727    | Benista     | 484375      | Anna          |

-- based on interview 163
SELECT F.id, AO.city AS origin, AD.city AS destination, F.hour FROM flights F
JOIN airports AO ON AO.id = F.origin_airport_id
JOIN airports AD ON AD.id = F.destination_airport_id
WHERE F.year = 2021 AND F.month = 7 AND F.day = 29
ORDER BY F.hour
LIMIT 2;
-- | 36 | Fiftyville | New York City | 8    |

SELECT P.seat, PPL.name, PPL.license_plate FROM passengers P
JOIN people PPL ON PPL.passport_number = P.passport_number
WHERE P.flight_id = 36;
(
    SELECT F.id FROM flights F
    WHERE F.year = 2021
    AND F.month = 7
    AND F.day = 29
    ORDER BY F.hour
    LIMIT 1
);
-- | 2A   | Doris  | M51FA04       |
-- | 3B   | Sofia  | G412CB7       |
-- | 4A   | Bruce  | 94KL13X       |
-- | 5C   | Edward | 130LD9Z       |
-- | 6C   | Kelsey | 0NTHK55       |X
-- | 6D   | Taylor | 1106N58       |X
-- | 7A   | Kenny  | 30G67EN       |
-- | 7B   | Luca   | 4328GD8       |

-- based on interview 193
SELECT PH.caller, PH.receiver, PH.duration
FROM phone_calls PH
WHERE PH.year = 2021 AND PH.month = 7 AND PH.day = 28;

