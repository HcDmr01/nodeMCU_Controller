--------------------------------- TR --------------------------
NodeMCU Tabanlı IoT Cihazı Kontrol Paneli
Bu proje, ESP8266 tabanlı bir NodeMCU kartı kullanarak bir IoT cihazını kontrol etmeyi amaçlar. Cihaz, çeşitli sensör verilerini toplar ve kullanıcıya bir web arayüzü üzerinden bu verileri sunar. Kullanıcılar, web arayüzü üzerinden cihazın çeşitli fonksiyonlarını kontrol edebilir.

Özellikler
Ana Şalter (Main Switch): Cihazın ana fonksiyonlarını açıp kapatmanızı sağlar.
Nozzle ve Pompa Kontrolleri: Nozzle ve pompa bileşenlerini ayrı ayrı açıp kapatmanızı sağlar.
DHT11 Sensörü: Sıcaklık ve nem verilerini ölçer ve web arayüzünde gösterir.
Kronometre: Nozzle açık kaldığı süreyi ölçer ve ekranda gösterir.
Yön Butonları: Cihaz üzerinde yönlendirme komutları vermenizi sağlar.
Gereksinimler
Donanım:

ESP8266 NodeMCU
DHT11 Sıcaklık ve Nem Sensörü
Çeşitli butonlar, röleler ve LED'ler
Yazılım:

Arduino IDE
ESP8266WiFi Kütüphanesi
DNSServer Kütüphanesi
ESP8266WebServer Kütüphanesi
EEPROM Kütüphanesi
DHT Kütüphanesi
Kurulum
Arduino IDE'yi Yükleyin: Arduino IDE'yi İndirin
ESP8266 Kart Desteği Ekleyin:
Arduino IDE'ye gidin: Dosya > Tercihler
"Ekstra Kart Yöneticisi URL'leri" alanına http://arduino.esp8266.com/stable/package_esp8266com_index.json ekleyin.
Araçlar > Kart > Kart Yöneticisi bölümünden ESP8266 kartlarını aratın ve yükleyin.
Gerekli Kütüphaneleri Kurun: Araçlar > Kütüphane Yöneticisi'ne gidin ve aşağıdaki kütüphaneleri kurun:
ESP8266WiFi
DNSServer
ESP8266WebServer
EEPROM
DHT
Kodunuzu NodeMCU'ya Yükleyin:
Arduino IDE'de Dosya > Aç ile bu proje dosyasını açın.
NodeMCU'yu bilgisayarınıza bağlayın ve Araçlar > Port bölümünden doğru portu seçin.
Kodu yüklemek için Yükle butonuna basın.
Kullanım
Cihazı Başlatın: NodeMCU'yu enerjiye bağlayın.
Web Arayüzüne Bağlanın:
NodeMCU'nun yayınladığı Wi-Fi ağına bağlanın (SSID_NAME ve PASS ile).
Bir tarayıcı açın ve http://172.0.0.1 adresine gidin.
Cihazı Kontrol Edin:
Web arayüzü üzerinden Ana Şalter, Nozzle ve Pompa kontrollerini kullanarak cihazı yönetebilirsiniz.
Yön butonları ile cihazı yönlendirme komutları verebilirsiniz.
Kod Yapısı
index(): Web arayüzünün HTML içeriğini oluşturur.
refresh(): Sayfayı yeniler ve güncellenmiş verileri sunar.
mainSwitchOn(): Ana şalteri açar.
mainSwitchOff(): Ana şalteri kapatır.
nuzzleOn(): Nozzle'ı açar.
nuzzleOff(): Nozzle'ı kapatır.
pompaOn(): Pompayı açar.
pompaOff(): Pompayı kapatır.

Katkıda Bulunma
Katkılarınızı bekliyoruz! Lütfen katkıda bulunmak isterseniz bir pull request gönderin veya bir konu açın.

NodeMCU-Based IoT Device Control Panel
This project aims to control an IoT device using an ESP8266-based NodeMCU board. The device collects various sensor data and presents it to the user via a web interface. Users can control various functions of the device through this web interface.

------------------- ENG --------------------------------

Features
Main Switch: Allows you to turn the device's main functions on or off.
Nozzle and Pump Controls: Allows you to separately turn the nozzle and pump components on or off.
DHT11 Sensor: Measures temperature and humidity data and displays it on the web interface.
Timer: Measures the duration the nozzle remains on and displays it on the screen.
Direction Buttons: Allows you to issue directional commands to the device.
Requirements
Hardware:

ESP8266 NodeMCU
DHT11 Temperature and Humidity Sensor
Various buttons, relays, and LEDs
Software:

Arduino IDE
ESP8266WiFi Library
DNSServer Library
ESP8266WebServer Library
EEPROM Library
DHT Library
Installation
Install Arduino IDE: Download Arduino IDE
Add ESP8266 Board Support:
Go to Arduino IDE: File > Preferences
In the "Additional Board Manager URLs" field, add http://arduino.esp8266.com/stable/package_esp8266com_index.json.
Go to Tools > Board > Board Manager, search for ESP8266, and install it.
Install Required Libraries: Go to Tools > Library Manager and install the following libraries:
ESP8266WiFi
DNSServer
ESP8266WebServer
EEPROM
DHT
Upload Your Code to NodeMCU:
Open this project file in Arduino IDE via File > Open.
Connect your NodeMCU to your computer and select the correct port under Tools > Port.
Click the Upload button to upload the code.
Usage
Start the Device: Power up the NodeMCU.
Connect to the Web Interface:
Connect to the Wi-Fi network broadcasted by the NodeMCU (with SSID_NAME and PASS).
Open a browser and navigate to http://172.0.0.1.
Control the Device:
Use the web interface to control the Main Switch, Nozzle, and Pump.
Use the direction buttons to issue directional commands to the device.
Code Structure
index(): Generates the HTML content for the web interface.
refresh(): Refreshes the page and provides updated data.
mainSwitchOn(): Turns the main switch on.
mainSwitchOff(): Turns the main switch off.
nuzzleOn(): Turns the nozzle on.
nuzzleOff(): Turns the nozzle off.
pompaOn(): Turns the pump on.
pompaOff(): Turns the pump off.
Contributing
We welcome contributions! If you'd like to contribute, please submit a pull request or open an issue.
