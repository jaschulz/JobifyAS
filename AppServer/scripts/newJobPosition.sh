echo '{"name": "sSr COBOL developer","description": "a Semi-Senior COBOL developer"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/software

