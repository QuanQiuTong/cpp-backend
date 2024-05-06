create table problems(
    id int primary key,

    category varchar(255) not null,
    personality TEXT,
    history TEXT,
    problem TEXT not null,
    solution1 TEXT not null,
    solution2 TEXT not null
);

CREATE TABLE annotation(
    id SERIAL PRIMARY KEY,
    problem_id int not null,
    user_id int not null,

    judgement VARCHAR not null,
    reason text not null,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (problem_id) REFERENCES problems(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE users(
    id SERIAL PRIMARY KEY,
    
    username VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL
);