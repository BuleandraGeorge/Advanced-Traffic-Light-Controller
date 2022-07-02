char TEMPLATE[] = "<!DOCTYPE html><html><head><meta http-equiv = \"refresh\" content = \"10\"> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel=\"stylesheet\" href=\"style.css\"> <title>ESP8266</title></head><body> <h1 class=\"text-center\"><span class=\"shadow p-1 rounded bg-lightblue\">ESP8266 REMOTE CONTROLLER</span></h1> <div class=\"container\"> <div class=\"row\"> <div class=\"col-sm-12 col-md-6 p-1\"> <div class=\"card shadow\"> <div class=\"card-body\"> <div class=\"card-title text-center\"><h2>REMOTE</h2></div><div class=\"row\"> <div class=\"col-sm-6\"><h4 class=\"text-center m-0\">TRAFFIC LIGHTS MODES</h4></div><div class=\"col-sm-6\"><h4 class=\"text-center m-0\">3 COLORED LED MODES</h4></div></div><hr/> <div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/equal_priority\"> <input type=\"submit\" value=\"EQUAL PRIORITY\"></form> </div><div class=\"col-sm-6 text-center p-1\"> <form action=\"/ledoff\"> <input class=\"text-white bg-black\" type=\"submit\" value=\"LED OFF \"></form></div></div><div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/set1_priority\"> <input type=\"submit\" value=\"SET 1 PRIORITY\"></form> </div><div class=\"col-sm-6 text-center p-1\"> <form action=\"/led_amber\"> <input class=\"bg-amber text-black\" type=\"submit\" value=\"HAZARD\"></form> </div></div><div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/set2_priority\"> <input type=\"submit\" value=\"SET 2 PRIORITY\"></form> </div><div class=\"col-sm-6 text-center p-1 bg-blue, text-white\"> <form action=\"/led_blue\"> <input class=\"bg-blue text-white\" type=\"submit\" value=\"EMERGENCY\"></form> </div></div><div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/maintenance\"> <input type=\"submit\" value=\"MAINTENANCE\"></form> </div><div class=\"col-sm-6 text-center p-1\"> <form action=\"/led_green\"> <input class=\"bg-green text-black\" type=\"submit\" value=\"DOCTOR ON CALL\"></form> </div></div><div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/barrier_crossing\"> <input type=\"submit\" value=\"BARRIER CROSSING\"></form> </div><div class=\"col-sm-6 text-center p-1\"> <form action=\"/led_red\"> <input class=\"bg-red text-white\" type=\"submit\" value=\"POLICE CAR\"></form> </div></div><div class=\"row\"> <div class=\"col-sm-6 text-center p-1\"> <form action=\"/motor_racing\"> <input type=\"submit\" value=\"MOTOR RACING\"></form> </div><div class=\"col-sm-6\"> </div></div><div class=\"row\"> <div class=\"col-sm-12 text-center p-1\"> <form action=\"/get_status\"> <input type=\"submit\" value=\"GET STATUS\"></form></div></div></div></div></div><div class=\"col-sm-12 col-md-6 p-1\"> <div class=\"card shadow\"> <div class=\"card-body %s\"> <div class=\"card-title text-center m-0\"><h2>STATUS</h2></div><div class=\"row\"> <div class=\"col-sm-12 m-0\"><h4 class=\"text-center m-0\">TRAFFIC LIGHTS</h4></div></div><hr/> <div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-0\">CURRENT MODE</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-0\">%s</h5></div></div><div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-0\">NEXT MODE</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-0\">%s</h5></div></div><div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-0\">CURRENT CYCLE</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-0\">%s</h5></div></div><hr/> <div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-1\">3 COLORED LED</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-1\">%s</h5></div></div><div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-0\">TEMPERATURE</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-0\">%s</h5></div></div><div class=\"row\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-1\">TILT STATUS</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-1\">%s</h5></div></div><div class=\"row mb-1\"> <div class=\"col-sm-6 text-center \"><h5 class=\" text-center m-0\">TIME ACTIVE</h5></div><div class=\"col-sm-6 text-center \"><h5 class=\"text-center m-0\">%s</h5></div></div></div></div></div></div></div></body></html>";
char INDEX_HTML[4500];
