require "networkutils"
require "vector"

peers = {}
peerID = 0
function onPeerConnect(id)
	print("Got connection from peer "..id)
	network.sendPacket(id, "player".. peerID)
	local p = {}
	p.id = id
	p.model = GO.loadIQM("mr_fixit.iqm","player".. peerID)
	p.model:setConvexBody()
	p.model:setMass(1)
	p.model:setPos(0,10,0)
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
	right = Vector.cross(fwd, Vector.create(0,1,0))
	if data[1] == "forward" then
		p.model:setVelocity(fwd:get())
	elseif data[1] == "backward" then
		p.model:setVelocity(Vector.scalarMul(-1,fwd):get())
	elseif data[1] == "right" then
		p.model:setVelocity(right:get())
	elseif data[1] == "left" then
		p.model:setVelocity(Vector.scalarMul(-1,right):get())
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
	floor:setMass(0)
	floor:setScale(5,1,5)
	floor:setPos(0,-5.5,0)
end

delta = 0
function update(dt)
	delta = dt
end
