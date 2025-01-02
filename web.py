import ffmpeg

# HLS Stream URL
hls_url = "https://hls.animeindy.com:8443/vid/1CQPbr6JGZ/video.mp4/chunk.m3u8?nimblesessionid=27081552"

# Output File
output_file = "output.mp4"

# Use FFmpeg to process the HLS stream with multi-threading optimizations
try:
    (
        ffmpeg
        .input(hls_url, protocol_whitelist='file,http,https,tcp,tls')  # Allow multiple protocols
        .output(output_file, codec='copy', hls_flags='delete_segments')  # Use flags to handle segments efficiently
        .global_args('-threads', '8')  # Enable multi-threading with 8 threads
        .global_args('-loglevel', 'error')  # Suppress detailed logs
        .run()
    )
    print(f"Download completed: {output_file}")
except ffmpeg.Error as e:
    print("An error occurred while processing the stream:", e.stderr.decode())
