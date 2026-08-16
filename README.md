# TeleType Layer Security

Here is my very basic---and likely a bit insecure---implementation of tls
protocol, which I am intending to use with my other project, in which my friend
and I build typewriter consoles for some reason. The goal of this project is
not security, (though there will be some) but rather it is just because I want
a minimum barrier to just using up all the paper in the typewriters. If you
hack this I suppose you are welcome to use them... Perhaps just print out
documentation of how you did if you do, I'd be curious to know.

## Data encoding
Uniform will have roughly similar hamming distance from any two samples... as
well as similar number of 1's and 0's 

# Protocol
1. Client establishes connection, requests public key
2. Server replies with key, requests client authentication key
3. Client sends auth key
4. Server acknowledge, sends status of printer
5. Client sends message
6. Server closes connection
