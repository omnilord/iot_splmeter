import sqlite3
import paho.mqtt.client as mqtt
import json
import signal
import sys

DB = sqlite3.connect('soundsensordata.db')
DB.execute('create table if not exists readings (sensor unsigned int not null, iot_time unsigned int, t timestamp default CURRENT_TIMESTAMP, adc int not null default 0)')
DB.execute('create table if not exists events (sensor unsigned int not null, iot_time unsigned int, t timestamp default CURRENT_TIMESTAMP, desc text)')

def connect_sensor(sensorid, data):
    """construct the connect event SQL insert and execute it"""
    DB.execute("insert into events values (?, ?, CURRENT_TIMESTAMP, 'connect')", (int(sensorid), int(data['t']),))
    DB.commit()

def disconnect_sensor(sensorid, data):
    """construct the disconnect event SQL insert and execute it"""
    DB.execute("insert into events values (?, ?, CURRENT_TIMESTAMP, 'disconnect')", (int(sensorid), int(data['t']),))
    DB.commit()

def sensor_reading(sensorid, data):
    """construct the reading SQL insert and execute it"""
    DB.execute('insert into readings values (?, ?, CURRENT_TIMESTAMP, ?)', (int(Sensorid), data['adc'],))
    DB.commit()

def sensor_state(sensorid, data):
    """route sensor state date to connect/disconnect"""
    pass

MSG_ROUTER = {
    'state': sensor_state,
    'connect': connect_sensor,
    'disconnect': disconnect_sensor,
    'reading': sensor_reading
}

def on_connect(client, userdata, flags, rc):
    """when connected to the MQTT broker, subscribe to the sensor topics"""
    print('Connected with result code '+str(rc))
    client.subscribe('/soundsensors/+/+')

def on_message(client, userdata, msg):
    """handle receiving messages on subscribed topics"""
    _, _, sensor_id, message_type = msg.topic.split('/')
    payload = str(msg.payload)
    print(sensor_id+' reported '+message_type+': '+payload)

    try:
        data = json.loads(payload)
        fn = MSG_ROUTER.get(message_type)
        fn(sensor_id, data)
    except:
        err = sys.exc_info()[0]
        print(str(err))


client = mqtt.Client(client_id="mqtt_local_proxy_pi4")
client.on_connect = on_connect
client.on_message = on_message
client.connect('localhost', 1883, 60)

if __name__ == "__main__":
    """Unsure if this is the best way to do this, but meh, it works."""

    try:
        client.enable_logger()
        client.loop_forever()
    except KeyboardInterrupt:
        print('terminating...')
    finally:
        client.disconnect()
        print('Done.')