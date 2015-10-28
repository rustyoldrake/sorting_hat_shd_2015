import argparse
from argparse import Namespace as arguments
import audiotools
import base64
import json
import sys
import requests
import urllib
import pyaudio
import RPi.GPIO as GPIO
import yaml
import subprocess
import time
import wave

CHUNK = 512
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 8
OUTPUT_FILENAME = "output_{}".format(int(time.time()))
WAVE_OUTPUT_FILENAME = "{}.wav".format(OUTPUT_FILENAME)

DEVICE_INPUT = 2
DEVICE_OUTPUT = 2
WAVE_SUFFIX='_v06'
INTRO_WAVE='TryMeOn_v06.wav'

def process_arguments():
  parser = argparse.ArgumentParser(description='Sorting Hat')
  parser.add_argument("-w", "--wave", default=None, help='audio file override')
  parser.add_argument("-y", "--yunip", default='172.31.34.184', help='yun ip')
  parser.add_argument("-t", "--tvip", default='172.31.32.222:3000', help='tv ip')
  parser.parse_args(namespace=arguments)

def play_wave(wavefile):
  subprocess.call(['aplay', wavefile])

def record():
  p = pyaudio.PyAudio()

  stream = p.open(format=FORMAT,
                  channels=CHANNELS,
                  rate=RATE,
                  input=True,
                  input_device_index = DEVICE_INPUT,
                  frames_per_buffer=CHUNK)

  print("* recording")

  frames = []

  for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
      data = stream.read(CHUNK)
      frames.append(data)

  print("* done recording")

  stream.stop_stream()
  stream.close()
  p.terminate()

  wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
  wf.setnchannels(CHANNELS)
  wf.setsampwidth(p.get_sample_size(FORMAT))
  wf.setframerate(RATE)
  wf.writeframes(b''.join(frames))
  wf.close()
  return WAVE_OUTPUT_FILENAME

def extract_house(json_data):
  """
    {
      "classifier_id" : "9EF9A9-nlc-999",
      "url" : "https://gateway.watsonplatform.net/natural-language-classifier/api/v1/classifiers/9EF9A9-nlc-999",
      "text" : "I am Cedric I'm Cedric Diggory I am a good guy ",
      "top_class" : "Hufflepuff",
      "classes" : [ {
        "class_name" : "Hufflepuff",
        "confidence" : 0.9423073689977156
      }, {
        "class_name" : "Gryffindor",
        "confidence" : 0.03710819969326022
      }, {
        "class_name" : "Ravenclaw",
        "confidence" : 0.015521308524388756
      }, {
        "class_name" : "Slytherin",
        "confidence" : 0.005063122784635296
      } ]
    }

  """
  data = json.loads(json_data)

  return data['classes'][0]['class_name']

def invoke_event(house):
  url = 'http://{}/arduino/{}/1'.format(arguments.yunip,(extract_house(house)).lower())
  try:
    r = requests.get(url, timeout=2.0)
  except:
    pass

  sorted_response_url = 'http://{}/sorted_response?data={}'.format(arguments.tvip, house)

  try:
    r2 = requests.get(sorted_response_url, timeout=2.0)
  except:
    pass

def invoke_yun_event(route):
  sorted_response_url = 'http://{}/{}/1'.format(arguments.yunip, route)
  try:
    r2 = requests.get(sorted_response_url, timeout=2.0)
  except:
    pass

def invoke_tv_event(route):
  sorted_response_url = 'http://{}/{}'.format(arguments.tvip, route)
  try:
    r2 = requests.get(sorted_response_url, timeout=2.0)
  except:
    pass


def main():
  config = yaml.load(open("the_hat.yml", 'r'))

  invoke_tv_event('asking_question')
  invoke_yun_event('question')
  play_wave(INTRO_WAVE)

  if arguments.wave is None:
    filename = record()
  else:
    filename = arguments.wave

  """
  curl -u 9999999-78a3-402f-930a-8f3ff013dde4:ABC123ABC123 -H "content-type: audio/wav" --data-binary @"output_1445720782.wav" https://stream.watsonplatform.net/speech-to-text/api/v1/recognize



{
   "results": [
      {
         "alternatives": [
            {
               "confidence": 0.9159336686134338,
               "transcript": "I am clever I am curious clever intelligence I'm a Smarty pants "
            }
         ],
         "final": true
      }
   ],
   "result_index": 0
}

txt2speech_user
txt2speech_passwd
classifier_user
classifier_passwd


  """
  invoke_yun_event('thinking')
  url = 'https://stream.watsonplatform.net/speech-to-text/api/v1/recognize'
  username = config['txt2speech_user']
  password = config['txt2speech_passwd']
  headers={'Content-Type': 'audio/wav'}
  audio = open(filename, 'rb')
  r = requests.post(url, data=audio, headers=headers, auth=(username, password))

  data = json.loads(r.text)

  transcript = '{}'.format(data['results'][0]['alternatives'][0]['transcript'])

  print "{}".format(transcript)

  classifier_url = \
   'https://gateway.watsonplatform.net/natural-language-classifier/api/v1/classifiers/99999-nlc-999/classify?text={}'\
     .format(urllib.quote_plus(transcript))

  r_classifier = requests.get(classifier_url, auth=(config['classifier_user'], config['classifier_passwd']))

  invoke_event(r_classifier.text)
  house = extract_house(r_classifier.text)
  print(house)
  play_wave('{}{}.wav'.format(house.lower(), WAVE_SUFFIX))

if __name__ == "__main__":
  process_arguments()
  GPIO.setmode(GPIO.BCM)

  GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_UP)

  while True:
    input_state = GPIO.input(18)
    if input_state == False:
      main()
