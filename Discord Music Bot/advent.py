import discord, time, youtube_dlc, asyncio, threading
from hashlib import sha256

greeting = {"hi", "hello", "yo", "hey", "long time no see", "it's been a while", "been a while", "good morning", "good evening", "good afternoon", "hiya", "morning", "good night", "good to see you", "nice to see you", "great to see you"}
voice_connected = False
vclient = None
player = None
channel = None
song_queue = []
queue_thread = None
loop2 = None
id_set = True

youtube_dlc.utils.bug_reports_message = lambda: ''

ytdl_format_options = {
    'format': 'bestaudio/best',
    'outtmpl': '%(extractor)s-%(id)s-%(title)s.%(ext)s',
    'restrictfilenames': True,
    'noplaylist': True,
    'nocheckcertificate': True,
    'ignoreerrors': False,
    'logtostderr': False,
    'quiet': True,
    'no_warnings': True,
    'default_search': 'auto',
    'source_address': '0.0.0.0' # bind to ipv4 since ipv6 addresses cause issues sometimes
}

ffmpeg_options = {
    'options': '-vn'
}

ytdl = youtube_dlc.YoutubeDL(ytdl_format_options)

class YTDLSource(discord.PCMVolumeTransformer):
    def __init__(self, source, *, data, volume=0.5):
        super().__init__(source, volume)

        self.data = data

        self.title = data.get('title')
        self.url = data.get('url')

    @classmethod
    async def from_url(cls, url, *, loop=None, stream=False):
        try:
            global song_queue
            global id_set
            loop = loop or asyncio.get_event_loop()
            data = await loop.run_in_executor(None, lambda: ytdl.extract_info(f"ytsearch1:{url} --verbose", download=not stream))
            if 'entries' in data:
                # take first item from a playlist
                data = data['entries'][0]

            filename = data['url'] if stream else ytdl.prepare_filename(data)
            #find a fix for queue url since data keeps changing
            if(id_set):
                song_queue[0] = "https://www.youtube.com/watch?v=" + data['id']
                id_set = False
            return cls(discord.FFmpegPCMAudio(filename, **ffmpeg_options), data=data)
        except:
            pass


def authorIsBot(id):
    if(sha256(str(id).encode()).hexdigest() == "80ac4f165f476822cfdb7bf7476da754f486e10435e063fd84a24a47158938fb"):
        return True
    return False

def isGreeting(message):
    for g in greeting:
        if((g + " <@!763055542482501654>") in message):
            return g
    return False

def check_queue(error):
    global queue_thread
    global id_set
    id_set = True
    song_queue.pop(0)
    loop2 = asyncio.new_event_loop()
    queue_thread = threading.Thread(target=queue_func)
    queue_thread.start()
    
            
def queue_func():
    global voice_connected
    global vclient
    global player
    global song_queue
    global channel
    global client
    global loop2
    
    
    asyncio.set_event_loop(loop2)
    #TODO SET SONG QUEUE TO NONE IF SONGS ARE FINISHED
    if song_queue:
    #try:
        player = asyncio.run(YTDLSource.from_url(song_queue[0], loop=loop2))
        vclient.play(player, after=check_queue)
        asyncio.run_coroutine_threadsafe(channel.send(f"Now playing - {player.title}.\nEnjoy!"), client.loop)
        print(song_queue)
    #except:
        #song_queue.pop(0)
        #await channel.send("Look what we have in the queue... An error.")
        
class BotClient(discord.Client):
        
    async def on_ready(self):
        print('Logged on as {0}!'.format(self.user))

    async def on_message(self, message):
        global voice_connected
        global vclient
        global player
        global song_queue
        global channel
        
        print('Message from {0.author}: {0.content}'.format(message))
        channel = message.channel
        if not authorIsBot(message.author.id):
            
            #say hi
            greet_mes = isGreeting(message.content.lower())
            if(greet_mes):
                await channel.send(f"{greet_mes[0].upper() + greet_mes[1:]}, <@!{message.author.id}>")
            
            #connect voice chat
            elif(message.content.startswith("$connect")):
            
                authvoice = message.author.voice
                
                if not authvoice:
                    await channel.send(f"You have to be in a voice channel to use this command <@!{message.author.id}>.")
                    
                
                #elif not perms & speak & etc.:
                    #await channel.send("I need voice channel permissions to complete the task.")
                    
                else:
                    #no need since the bot is admin
                    #perms = discord.Permissions()
                    #print(perms)
                    voice_connected = True
                    await channel.send("I have connected to the voice channel.")
                    vclient = await authvoice.channel.connect()
                    
                    
            #disconnect voice chat
            elif(message.content.startswith("$disconnect")):
                if not voice_connected:
                    await channel.send("I have to be connected to a channel to be able to disconnect.")
                    
                else:
                    await vclient.disconnect()
                    await channel.send("I have disconnected from the voice channel.")
                    voice_connected = False
                    vclient = None
                    player = None
                    channel = None
                    song_queue = []  
                    
            elif(message.content.startswith("$play; ")):
                if not voice_connected:
                    await channel.send(f"Stop messing around and get me into a voice channel <@!{message.author.id}>.")
                else:
                    #try:
                    url_list = message.content.split("; ")[1:]
                    for url in url_list:
                        song_queue.append(url)
                        
                    if not vclient.is_playing():
                        player = await YTDLSource.from_url(song_queue[0], loop=self.loop)
                        vclient.play(player, after=check_queue)
                        await channel.send(f"Now playing - {player.title}.\nEnjoy!")
                    #except:
                        #song_queue.pop(0)
                        #await channel.send("You should try better than that.")
                        
            elif(message.content.startswith("$pause")):
                if vclient.is_playing():
                    vclient.pause()
                    await channel.send("I paused the audio.")
                else:
                    await channel.send("You can't fool me like that.")
                
            elif(message.content.startswith("$resume")):
                if vclient.is_paused():
                    vclient.resume()
                    await channel.send("I resumed the audio.")
                else:
                    await channel.send("You can't fool me like that.")
                
            elif(message.content.startswith("$np")):
                if vclient.is_playing():
                    await channel.send(f"Now playing - {player.title}.")
                else:
                    await channel.send("You can't fool me like that.")
                
            elif(message.content.startswith("$skip")):
                if vclient.is_playing():
                    try:
                        vclient.stop()
                        await channel.send("I skipped the current song. Let's see what we have next.")
                        if(len(song_queue) == 0):
                            await channel.send("I'm afraid the queue is empty.")
                        else:
                            player = await YTDLSource.from_url(song_queue[0], loop=self.loop)
                            vclient.play(player, after=check_queue)
                            await channel.send(f"Now playing - {player.title}.\nEnjoy!")
                    except:
                        song_queue.pop(0)
                        await channel.send("You should try better than that.")
                else:
                    await channel.send("You can't fool me like that.")
                    
            elif(message.content.startswith("$playskip; ")):
                if vclient.is_playing():
                    try:
                        vclient.stop()
                        song_queue.insert(0, message.content.split("; ")[1])
                        player = await YTDLSource.from_url(song_queue[0], loop=self.loop)
                        vclient.play(player, after=check_queue)
                        await channel.send(f"I skipped the current song. Now playing - {player.title}.\nEnjoy!")
                    except:
                        song_queue.pop(0)
                        await channel.send("You should try better than that.")
                else:
                    await channel.send("You can't fool me like that.")
            elif(message.content.startswith("$queue")):
                if len(song_queue) > 5:
                    for i in range(5):
                        await channel.send(f"{song_queue[i]}")
                    await channel.send("This is not all I've got.")
                    await channel.send(f"Total songs in queue: {len(song_queue)}")
                else:
                    for url in song_queue:
                        await channel.send(f"{url}")
                    await channel.send("End of queue.")
                
                    
            #if no such command
            else:
                if(message.content[0] == '$'):
                    await channel.send(f"{message.content.split(' ')[0]} is not a recognized command.")
        

client = BotClient()
client.run('NzYzMDU1NTQyNDgyNTAxNjU0.X3yI2g.zNZGxakx1xhZNd7HMZfnU_DFrQc')
    