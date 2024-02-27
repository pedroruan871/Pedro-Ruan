-- Keep a log of any SQL queries you execute as you solve the mystery.



-- Car plates leaving the courthouse parking lot after the crime (less than 10 minutes)
SELECT name
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM courthouse_security_logs
    WHERE hour = 10
    AND minute > 15
    AND minute < 25
    AND year = 2020
    AND day = 28
    AND month = 7
)
AND id IN
(   -- People who have made transactions before the crime
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        -- Transactions made before the crime
        SELECT account_number
        FROM atm_transactions
        WHERE transaction_type = 'withdraw'
        AND day = 28
        AND month = 7
        AND year = 2020
        AND atm_location = 'Fifer Street'
    )
)
AND phone_number IN
(
    --Phone calls made after the theft when the thief was leaving
    SELECT caller
    FROM phone_calls
    WHERE duration < 60
    AND day = 28
    AND month = 7
    AND year = 2020
)
AND passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id IN
    (
        SELECT id
        FROM flights
        WHERE year = 2020
        AND month = 7
        AND day = 29
        AND hour =
        (
            SELECT min(hour)
            FROM flights
            WHERE year = 2020
            AND month = 7
            AND day = 29
        )
        AND origin_airport_id =
        (
            SELECT id
            FROM airports
            WHERE city = 'Fiftyville'
        )
    )
)
;

SELECT name
FROM people
WHERE phone_number =
(
    SELECT receiver
    FROM phone_calls
    WHERE day = 28
    AND month = 7
    AND year = 2020
    AND duration < 60
    AND caller =
    (
        SELECT phone_number
        FROM people
        WHERE license_plate IN
        (
            SELECT license_plate
            FROM courthouse_security_logs
            WHERE hour = 10
            AND minute > 15
            AND minute < 25
            AND year = 2020
            AND day = 28
            AND month = 7
        )
        AND id IN
        (   -- People who have made transactions before the crime
            SELECT person_id
            FROM bank_accounts
            WHERE account_number IN
            (
                -- Transactions made before the crime
                SELECT account_number
                FROM atm_transactions
                WHERE transaction_type = 'withdraw'
                AND day = 28
                AND month = 7
                AND year = 2020
                AND atm_location = 'Fifer Street'
            )
        )
        AND phone_number IN
        (
            --Phone calls made after the theft when the thief was leaving
            SELECT caller
            FROM phone_calls
            WHERE duration < 60
            AND day = 28
            AND month = 7
            AND year = 2020
        )
        AND passport_number IN
        (
            SELECT passport_number
            FROM passengers
            WHERE flight_id IN
            (
                SELECT id
                FROM flights
                WHERE year = 2020
                AND month = 7
                AND day = 29
                AND hour =
                (
                    SELECT min(hour)
                    FROM flights
                    WHERE year = 2020
                    AND month = 7
                    AND day = 29
                )
                AND origin_airport_id =
                (
                    SELECT id
                    FROM airports
                    WHERE city = 'Fiftyville'
                )
            )
        )
    )
)
;

SELECT city
FROM airports
WHERE id IN
    (
        SELECT destination_airport_id
        FROM flights
        WHERE year = 2020
        AND month = 7
        AND day = 29
        AND hour =
        (
            SELECT min(hour)
            FROM flights
            WHERE year = 2020
            AND month = 7
            AND day = 29
        )
        AND origin_airport_id =
        (
            SELECT id
            FROM airports
            WHERE city = 'Fiftyville'
        )
    )
