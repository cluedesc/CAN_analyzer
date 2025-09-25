CREATE TABLE can_messages (
    id BIGINT PRIMARY KEY,
    name TEXT,
    last_seen TIMESTAMP,
    message_count BIGINT,
    last_data BYTEA
);  