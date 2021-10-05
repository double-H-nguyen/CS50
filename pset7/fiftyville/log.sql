-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- Determine relevant reports and get description
SELECT description
FROM crime_scene_reports
WHERE street = 'Chamberlin Street'
    AND year = 2020 AND month = 7 AND day = 28;
-- description:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Get interviews transcript
SELECT name, transcript
FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28;
-- name | transcripts
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- Find which car the thief got into
-- Car left within 10 mins of the theft (10:05am to 10:25am)
-- Get list of people who own the license_plate that drove out during this time
SELECT name
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM courthouse_security_logs
    WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10
        AND minute >= 5 AND minute <= 25
    )
ORDER BY name ASC;
-- Whoever own the car is the thief or accomplice
-- name
-- Amber
-- Brandon
-- Danielle
-- Elizabeth
-- Ernest
-- Evelyn
-- Patrick
-- Roger
-- Russell
-- Sophia


-- Understand how atm_transactions works
SELECT DISTINCT(transaction_type) FROM atm_transactions;
SELECT DISTINCT(atm_location) FROM atm_transactions;

-- Get list of people who withdrawed money on the day of the theft
SELECT p.name
FROM bank_accounts AS b JOIN people AS p ON b.person_id = p.id
WHERE b.account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2020 AND month = 7 AND day = 28
        AND atm_location = 'Fifer Street'
        AND transaction_type = 'withdraw'
    )
ORDER BY p.name ASC;
-- One of these people is the thief
-- name
-- Bobby
-- Danielle
-- Elizabeth
-- Ernest
-- Madison
-- Roy
-- Russell
-- Victoria


-- Get list of callers who made a phone call for less than a minute
SELECT name
FROM people
WHERE phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2020 AND month = 7 AND day = 28
        AND duration < 60
    )
ORDER BY name ASC;
-- One of these people is the thief
-- name
-- Bobby
-- Ernest
-- Evelyn
-- Kimberly
-- Madison
-- Roger
-- Russell
-- Victoria

-- Get list of receiver who made a phone call for less than a minute
SELECT name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2020 AND month = 7 AND day = 28
        AND duration < 60
    )
ORDER BY name ASC;
-- One of these people is the accomplice
-- name
-- Anna
-- Berthold
-- Doris
-- Jack
-- Jacqueline
-- James
-- Larry
-- Melissa
-- Philip


-- Find earliest flight out. The destination is where the thief escaped to. The person who purchased the flight is the accomplice.
-- Find earliest flight out of Fiftyville on July 29, 2020 (day after crime took place)
SELECT id, destination_airport_id
FROM flights
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
    AND year = 2020
    AND month = 7
    AND day = 29
ORDER BY hour ASC, minute ASC
LIMIT 1;
-- id | destination_airport_id
-- 36 | 4

-- Find where thief escaped to
SELECT city
FROM airports
WHERE id = 4;
-- city
-- London

-- Find list of people who were on the same flight as the thief
SELECT name
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
    )
ORDER BY name ASC;
-- One of these people is the thief
-- name
-- Bobby
-- Danielle
-- Doris
-- Edward
-- Ernest
-- Evelyn
-- Madison
-- Roger


-- We know the thief withdrawn money, is a caller who made a phone call for less than a minute, drove out with a car, and is on the flight
-- Find the theif!
SELECT name
FROM people
WHERE name IN (
    -- names on flight
    SELECT name
    FROM people
    WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36)
    )
AND name IN (
    -- withdrawn money
    SELECT p.name
    FROM bank_accounts AS b JOIN people AS p ON b.person_id = p.id
    WHERE b.account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'
        )
    )
AND name in (
    -- caller
    SELECT name
    FROM people
    WHERE phone_number IN (
        SELECT caller
        FROM phone_calls
        WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60
        )
    )
AND name in (
    -- own the car driven out of the court
    SELECT name
    FROM people
    WHERE license_plate IN (
        SELECT license_plate
        FROM courthouse_security_logs
        WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 5 AND minute <= 25
        )
    );
-- name
-- Ernest <-- HE IS THE THIEF!


-- We know the the accomplice was the receiver of Ernest's call
SELECT name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60
        AND caller = (SELECT phone_number FROM people WHERE name = 'Ernest')
    );
-- name
-- Berthold <-- HE IS THE ACCOMPLICE