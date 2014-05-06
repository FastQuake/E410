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
	p.fwd = 0
	p.right = 0
	p.model = GO.loadIQM("mr_fixit.iqm","player".. peerID)
	p.model:setBoxBody()
	p.model:setActivation(true)
	p.model:setMass(100)
	p.model:setPos(0,10,0)
	p.model:lockAxis(0,0,0)
	peerID = peerID + 1
	table.insert(peers,p)
end

function onReceivePacket(id, data)
	local out = ""
	for k,v in pairs(data) do
		out = out.." "..v
	end
	--print(out)
	local p = Peer.getPeer(peers, id)
	if data[1] == "move" then
		if data[2] == "forward" then
			p.fwd = 1
		elseif data[2] == "backward" then
			p.fwd = -1
		elseif data[2] == "right" then
			p.right = 1
		elseif data[2] == "left" then
			p.right = -1
		end
	elseif data[1] == "stop" then
		if data[2] == "forward" then
			p.fwd = 0
		elseif data[2] == "backward" then
			p.fwd = -0
		elseif data[2] == "right" then
			p.right = 0
		elseif data[2] == "left" then
			p.right = -0
		end
	elseif data[1] == "turn" then
		p.model:setRot(data[2],0,0)
	elseif data[1] == "cast" then
		local obj = GO.castRay(data[2],data[3],data[4],data[5],data[6],data[7],50)
		if obj ~= nil then
			print("Looking at: "..obj:getTag())
		end
	end
end

function onPeerDisconnect(id)
	print("peer "..id.." disconnected")
	local p = Peer.getPeer(peers, id)
	p.model:remove()
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
	for k,v in pairs(peers) do
		local fwd = Vector.create(v.model:getLookat())
		local right = Vector.cross(Vector.create(0,1,0),fwd)
		local out = Vector.scalarMul(v.fwd,fwd) + Vector.scalarMul(v.right,right)
		local vel = Vector.create(v.model:getVelocity())
		if out ~= Vector.create(0,0,0) then
			out = out:normalize()
			out = Vector.scalarMul(playerSpeed, out)
			out.y = vel.y
			v.model:setVelocity(out:get())
		else
			v.model:setVelocity(0,vel.y,0)
		end
	end
end
