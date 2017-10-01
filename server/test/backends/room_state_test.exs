defmodule Server.Backends.GameStateTest do
  use ExUnit.Case, async: true
  alias Server.GameState

  test "Room state exists after get_or_create_room_state." do
    game_state = GameState.get_or_create_game_state(1)
    assert room_state
  end

  test "Room state contains player list." do
    game_state = GameState.get_or_create_game_state(2)
    assert room_state[:player_list]
  end

  test "Room states are different if room ids are different." do
    game_state_one = GameState.get_or_create_game_state(1)
    game_state_two = GameState.get_or_create_game_state(2)
    refute room_state_one == room_state_two
  end

  test "Room state contains room id." do
    game_state = GameState.get_or_create_game_state(3)
    assert room_state[:room_id] == 3
  end
end
