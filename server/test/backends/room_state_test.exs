defmodule Server.Backends.RoomStateTest do
  use ExUnit.Case, async: true
  alias Server.RoomState

  test "Room state exists after get_or_create_room_state." do
    room_state = RoomState.get_or_create_room_state(1)
    assert room_state
  end

  test "Roomg state contains player list." do
    room_state = RoomState.get_or_create_room_state(2)
    assert room_state[:player_list]
  end

  test "Room states are different if room ids are different." do
    room_state_one = RoomState.get_or_create_room_state(1)
    room_state_two = RoomState.get_or_create_room_state(2)
    refute room_state_one == room_state_two
  end

  test "Room state contains room id." do
    room_state = RoomState.get_or_create_room_state(3)
    assert room_state[:room_id] == 3
  end
end
