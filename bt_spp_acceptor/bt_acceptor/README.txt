Acceptor Example:
https://github.com/espressif/esp-idf/blob/master/examples/bluetooth/bluedroid/classic_bt/bt_spp_acceptor/

Configuration in example:

//Defaults in sdkconfig file from github example://

# Override some defaults so BT stack is enabled
# and WiFi disabled by default in this example
CONFIG_BT_ENABLED=y
CONFIG_BTDM_CTRL_MODE_BLE_ONLY=n
CONFIG_BTDM_CTRL_MODE_BR_EDR_ONLY=y
CONFIG_BTDM_CTRL_MODE_BTDM=n
CONFIG_BT_CLASSIC_ENABLED=y
CONFIG_WIFI_ENABLED=n
CONFIG_BT_SPP_ENABLED=y
CONFIG_BT_BLE_ENABLED=n

Required configuration in idf menu:

//Component Component Config -> Bluetooth ->  Bluetooth -> Host://

Enable Bluedroid Dual-mode

//Component Component Config -> Bluetooth ->  Bluetooth -> Controller://

Check Enabled

//Component Component Config -> Bluetooth -> Controller options -> Bluetooth Controller Mode (BR/EDR/DUALMODE)://

Select BR/EDR Only

//Component Component Config -> Bluetooth -> Bluedroid://

Enable "Classic Bluetooth -> SPP"
Disable "Secure Simple Pairing"


