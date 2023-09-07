CREATE TABLE recent_proc_status (
    id SERIAL PRIMARY KEY,
    pid INT,
    ppid INT,
    cmd TEXT,
    status TEXT, 
    start_time TEXT
);