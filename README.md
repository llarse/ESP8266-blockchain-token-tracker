
# Project Title

ESP-8266 script that queries coingeckos API every 5 minutes via HTTPS to fetch the current market price and market cap for a token. This is preset to display the value of $KNDX but can be reconfigured as needed. The display used is a 0.91" OLED 128x32. 

## Setup

You will need to set your network credentials in line 9 and 10 of the code:
```
// Network credentials
const char ssid[] = ""; 
const char password[] = "";
```

If you wish to change the endpoint, modify the URL in line 13

```
// API endpoint
const char priceCheckEndpoint[] = "https://api.coingecko.com/api/v3/simple/token_price/ethereum?contract_addresses=0x7ca5af5ba3472af6049f63c1abc324475d44efc1&vs_currencies=usd&include_market_cap=true"; 
```

Now, simply load in your Arduino IDE, select the board and port, and upload to your board. (Some boards with ESP-8266 manually require them to be put into a serializable mode. Usually, you can do this by holding down the dedicated reset button while connecting it to the PC)
## Wiring
The Wiring for this is extremely simple. 

The SCL and SDA on the OLED display need to go to the GPIO5 and GPIO4 pins respectively. The VCC should go to the 3.3V supply. 

On most boards wiring would look like this:

OLED Pins        ESP-8266 Pins
---------        -------------
   GND    ----->   GND
\
   VCC    ----->   3.3V
\
   SCL    ----->   D1 (GPIO 5)
\
   SDA    ----->   D2 (GPIO 4)

Hook up to the board to power and your ready to go