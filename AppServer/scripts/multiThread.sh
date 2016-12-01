#!/bin/bash

curl -s -o skills http://localhost:8081/api/skills && echo 'skills' &
curl -s -o categories http://localhost:8081/api/categories && echo 'categories' &
curl -s -o user http://localhost:8081/api/users/lutziane@yahoo.com.ar && echo 'lutziane@yahoo.com.ar' &
curl -s -o contacts http://localhost:8081/api/users/js5810@hotmail.com/contacts && echo 'contacts' &
curl -s -o job_positions http://localhost:8081/api/job_positions && echo 'job_positions' &
curl -s -o ranking http://localhost:8081/api/ranking && echo 'ranking' &
curl -s -o skills http://localhost:8081/api/skills && echo 'skills' &
curl -s -o categories http://localhost:8081/api/categories && echo 'categories' &
curl -s -o user http://localhost:8081/api/users/lutziane@yahoo.com.ar && echo 'lutziane@yahoo.com.ar' &
curl -s -o contacts http://localhost:8081/api/users/js5810@hotmail.com/contacts && echo 'contacts' &
curl -s -o job_positions http://localhost:8081/api/job_positions && echo 'job_positions' &
curl -s -o ranking http://localhost:8081/api/ranking && echo 'ranking' &
wait
