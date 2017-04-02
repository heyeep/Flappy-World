defmodule Server.Backends.PlayerListTest do
  use ExUnit.Case, async: true
  alias Server.PlayerList

  setup do
    {:ok, player_list} = PlayerList.start_link(%{})
    {:ok, process: player_list}
  end

  test "New player has different server_id than existing one.",
    %{process: player_list} do
    # Create a player with a starting server_id of 0.
    player_one = PlayerList.new_player(player_list)
    player_two = PlayerList.new_player(player_list)
    assert player_one[:server_id] != player_two[:server_id]
  end

  test "Player count matches number of player creations.",
    %{process: player_list} do
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    assert Enum.count(PlayerList.get_players(player_list)) == 4
  end

  test "Player is not included when getting other players.",
    %{process: player_list} do
    a = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    other_players = PlayerList.get_other_players(player_list, a)
    refute Enum.member?(other_players, a)
  end

  test "Correct player is found if server_id matches",
    %{process: player_list} do
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    c = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    result = PlayerList.get_player(player_list, c[:server_id])
    assert c == result
  end

  test "No player is found if no server id matches",
    %{process: player_list} do
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    _ = PlayerList.new_player(player_list)
    result = PlayerList.get_player(player_list, -1)
    refute result
  end

  test "Updating player updates player.",
    %{process: player_list} do
    player = PlayerList.new_player(player_list)
    updated_player = Map.put(player, :x, 10) |> Map.put(:y, 80)
    PlayerList.update_player(player_list, updated_player)
    same_player = PlayerList.get_player(player_list, player[:server_id])
    assert same_player.x == 10
    assert same_player.y == 80
  end
end
