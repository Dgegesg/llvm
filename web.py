import ffmpeg

# HLS Stream URL
hls_url = "https://hls.animeindy.com:8443/vid/1CQPbr6JGZ/video.mp4/chunk.m3u8?nimblesessionid=27081552"

# Output File
output_file = "output.mp4"

# Use FFmpeg to process the HLS stream
try:
    (
        ffmpeg
        .input(hls_url)
        .output(output_file, codec='copy')  # Use 'copy' to avoid re-encoding
        .run()
    )
    print(f"Download completed: {output_file}")
except ffmpeg.Error as e:
    print("An error occurred while processing the stream:", e.stderr.decode())
