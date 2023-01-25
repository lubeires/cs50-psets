-- Information on transcription:
    -- Theft took place at 10:15am.
    -- Within 10 minutes thief left the parking lot.
    -- Earlier the thief withdraw money at the ATM on Leggett Street.
    -- Thief called someone for less than 1 minute and asked the other person to buy a ticket for the next day earliest flight.

SELECT name as thief
FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
JOIN bakery_security_logs
ON bakery_security_logs.license_plate = people.license_plate
-- Gets the people that exited the bakery's parking lot at described time
WHERE bakery_security_logs.id IN
(
    SELECT id
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25
)
-- Gets the people that made a withdrawal that day at the ATM on Legget Street
AND atm_transactions.id IN
(
    SELECT id
    FROM atm_transactions
    WHERE year =  2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw" AND atm_location = "Leggett Street"
)
-- Gets the people that made a less than a minute call that day
AND people.phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
)
-- Gets the people at the earliest flight leaving Fiftyville the next day (29)
AND people.passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id =
    (
        SELECT id FROM flights
        WHERE origin_airport_id =
        (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville"
        )
        AND year = 2021 AND month = 7 and day = 29
        ORDER BY hour, minute
        LIMIT 1
    )
);

-- Gets the destination city
SELECT city
FROM airports
WHERE id =
(
    SELECT destination_airport_id
    FROM flights
    WHERE origin_airport_id =
        (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville"
        )
        AND year = 2021 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
);

-- Gets the accomplice
SELECT name AS accomplice
FROM people
WHERE phone_number IN
(
    SELECT receiver
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller =
    (
        SELECT phone_number
        FROM people
        WHERE name = "Bruce"
    )
);