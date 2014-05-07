require "networkutils"

peers = {}
peerID = 0
function onPeerConnect(id)
	print("Got connection from peer "..id)
	network.sendPacket(id, "player".. peerID)
	local p = {}
	p.id = id
	p.model = GO.loadIQM("mr_fixit.iqm","player".. peerID)
	peerID = peerID + 1
	table.insert(peers,p)
end

function onReceivePacket(id, data)
	out = ""
	for k,v in pairs(data) do
		out = out.." "..v
	end
	--print(out)
	local p = Peer.getPeer(peers, id)
	if data[1] == "forward" then
		p.model:setRot(0,data[3],0)
		p.model:move(10*delta)
	elseif data[1] == "backward" then
		p.model:setRot(0,data[3],0)
		p.model:move(-10*delta)
	elseif data[1] == "right" then
		p.model:setRot(0,data[3],0)
		p.model:strafe(10*delta)
	elseif data[1] == "left" then
		p.model:setRot(0,data[3],0)
		p.model:strafe(-10*delta)
	end
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected\n")
end

function init()
	fixit = GO.loadIQM("mr_fixit.iqm","fixit")
	floor = GO.loadIQM("cube.iqm","floor")
	shit = GO.loadIQM("monkey.iqm","monkey")
	cube = GO.loadIQM("cube.iqm","cube")

	floor:setScale(20, 1, 20)
	floor:setPos(0, -0.2, 0)
	shit:setPos(6, 1.1, 0)
	cube:setScale(1.5,1.5,1.5)
	cube:setPos(-3,1,2)
end

delta = 0
function update(dt)
	delta = dt
end
