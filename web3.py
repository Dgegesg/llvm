import subprocess

# Define the URL of the HLS stream and the output file
url = "https://hls.animeindy.com:8443/vid/1CQPbr6JGZ/video.mp4/chunk.m3u8?nimblesessionid=27108364"
output_file = "output.mp4"

# Build the ffmpeg command
ffmpeg_command = [
    "ffmpeg",
    "-i", url,
    "-c", "copy",
    "-bsf:a", "aac_adtstoasc",
    output_file
]

# Run the ffmpeg command
try:
    subprocess.run(ffmpeg_command, check=True)
    print(f"Download completed: {output_file}")
except subprocess.CalledProcessError as e:
    print(f"An error occurred: {e}")
