wrk.method = "POST"
wrk.body   = '{ "token": "tuturu" }'
wrk.headers["Content-Type"] = "application/json"

--[[
wrk -c 32 -t 16 -s ./benchmark/Route.lua -d 10s http://localhost:3000/
--]]