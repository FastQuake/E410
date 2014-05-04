require "networkutils"
require "vector"

peers = {}
peerID = 0
playerSpeed = 10
function onPeerConnect(id)
	print("Got connection from peer "..id)
	network.sendPacket(id, "player".. peerID)
	local p = {}
	p.id = id
	p.model = GO.loadIQM("mr_fixit.iqm","player".. peerID)
	p.model:setBoxBody()
	p.model:setMass(10)
	p.model:setPos(0,10,0)
	p.model:lockAxis(0,0,0)
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
	fwd = Vector.create(p.model:getLookat())
	right = Vector.cross(Vector.create(0,1,0),fwd)
	if data[1] == "forward" then
		p.model:setVelocity(Vector.scalarMul(playerSpeed,fwd):get())
	elseif data[1] == "backward" then
		p.model:setVelocity(Vector.scalarMul(-playerSpeed,fwd):get())
	elseif data[1] == "right" then
		p.model:setVelocity(Vector.scalarMul(playerSpeed,right):get())
	elseif data[1] == "left" then
		p.model:setVelocity(Vector.scalarMul(-playerSpeed,right):get())
	elseif data[1] == "stop" then
		p.model:setVelocity(0,0,0)
	elseif data[1] == "turn" then
		p.model:setRot(data[2],0,0)
	end
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected\n")
end

function init()
	floor = GO.loadIQM("cube.iqm","cube")
	floor:setMass(0.0)
	floor:setScale(15,0.3,15)
	floor:setPos(0,-25.5,0)

	shit = GO.loadIQM("monkey.iqm","monkey")
	shit:setMass(1.0)
	shit:setPos(3,5,0)
	shit:setBoxBody()
end

delta = 0
function update(dt)
	delta = dt
end
