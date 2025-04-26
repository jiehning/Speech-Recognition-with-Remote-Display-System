import whisper
import sounddevice as sd
import scipy.io.wavfile
import socket

HOST = '192.168.51.164'
PORT = 4242

def record_audio(filename="voice.wav", duration=10, fs=48000):
    print("recording...")
    audio = sd.rec(int(duration * fs), samplerate=fs, channels=2, dtype='int32')
    sd.wait()
    scipy.io.wavfile.write(filename, fs, audio)
    print("Record Finished!")

def transcribe_audio(filename="voice.wav"):
    model = whisper.load_model("base")
    result = model.transcribe(filename)
    return result['text']

record_audio()
result_text = transcribe_audio()

with open("/home/pi/snd2txt/result.txt", "w", encoding="utf-8") as f:
    f.write(result_text)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(result_text.encode('utf-8'))
    print(f'Sent result...')

print(f"Reconginition results :{result_text}")
print("The result has been saved to 'result.txt'")
