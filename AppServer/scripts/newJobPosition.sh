echo '{"name": "trainee COBOL developer","description": "a trainee COBOL developer"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/software

