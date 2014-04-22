require "networkutils"
peers = {}

function onPeerConnect(address, port)
	print("Got connection from: "..address..":"..port.."\n")
	p = Peer.create(address,port)
	p.model = GO.loadIQM("mr_fixit.iqm","player")
	table.insert(peers,p)
end

forward = false
function onReceivePacket(address, port, data)
	p = Peer.getPeer(peers, Peer.create(address,port))
	if data[1] == "forward" then
		p.model:move(10*delta)
	elseif data[1] == "right" then
		p.model:turn(50*delta,0)
	elseif data[1] == "left" then
		p.model:turn(-50*delta,0)
	end
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected\n")
end

function init()
end

delta = 0
function update(dt)
	delta = dt
end
