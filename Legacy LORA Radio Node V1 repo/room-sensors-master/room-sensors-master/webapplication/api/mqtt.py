import paho.mqtt.client as mqtt
import base64
import json
import logging

from heatmap.secrets import TTN_BROKER, TTN_USERNAME, TTN_PASSWORD, TTN_TOPIC

def on_connect(client, userdata,flags, rc):
    client.subscribe(TTN_TOPIC)
    print("Connected with result code " + str(rc))

    
def on_message(client, userdata, message):
    from .models import Sensor, History

    try:
        #take string from mqtt message and return json object to display output in handler
        json_data = json.loads(message.payload.decode('utf-8'))
        print (json_data)

        #extract all necessary data into variables
        hardware_serial = int(json_data['hardware_serial'])
        
        #extract payload_fields from payload created in Payload_Formats function decoder
        payload_fields = (json_data['payload_fields'])     

        #extract temperature & humidity from payload_fields 
        temperature = (json_data['payload_fields'].get('celsius'))
            
        humidity = (json_data['payload_fields'].get('humid'))  

        #extract only value from payload_fields, arrived as {'ppm2: value'}
        co2_ppm = (json_data['payload_fields'].get('ppm2'))

        #Look from db for serialID that match the devID in payload
        sensor = Sensor.objects.get(serialID=hardware_serial)
          
        #create history object with properties
        history = History.objects.create(sensor=sensor, temperature=temperature, humidity=humidity, co2_ppm=co2_ppm)

        #check for items in payload_fields as there are two ttn apps sending different payload to database
        #extract values from payload_fields
        if 'celsius' in payload_fields:
            sensor.temperature = payload_fields.get('celsius')
    
        if 'humid' in payload_fields:
            sensor.humidity = payload_fields.get('humid')   
           
        if 'ppm2' in payload_fields:        
            sensor.co2_ppm = payload_fields.get('ppm2')  
           
        #extract name and assign to sensor property
        sensor.name = json_data['dev_id']


        #collects time and date payloads were sent (formatted on the front end, see /webapplication/front-end/src/components/sensors/sensors.jsx)
        metadata = (json_data['metadata'])
        time =  metadata['time']
        sensor.time = time

        #save data to tables
        sensor.save() 
        print(f"Sensor updated: {sensor}")

        history.save()
        print(f"History created: {history}")
        

    except UnicodeDecodeError as e:
        # print("Unicode error " + str(e))
        print(e)
    except Exception as e:
        # print("Error " + str(e))
        print(e)


client = mqtt.Client()
client.username_pw_set(TTN_USERNAME, password=TTN_PASSWORD)
client.on_connect = on_connect
client.on_message = on_message
client.connect(TTN_BROKER, 1883 , 60)


