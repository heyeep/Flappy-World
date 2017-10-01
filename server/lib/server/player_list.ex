defmodule Server.PlayerList do
  @moduledoc """
  A module to persist the list of players per GameChannel.
  """
  use GenServer

  @doc """
  Create a new Player, add it this module's state and return the Player.
  """
  def new_player(pid) do
    GenServer.call(pid, :new_player)
  end

  @doc """
  Return list of Players that this module contains.
  """
  def get_players(pid) do
    GenServer.call(pid, :get_players)
  end

  @doc """
  Return list of Players that are not the current Player.
  """
  def get_other_players(pid, player) do
    GenServer.call(pid, {:get_other_players, player})
  end

  def get_num_players(pid) do
    GenServer.call(pid, {:get_num_players})
  end

  @doc """
  Return player given server_id.
  """
  def get_player(pid, server_id) do
    GenServer.call(pid, {:get_player, server_id})
  end

  @doc """
  Update player.
  """
  def update_player(pid, player) do
    GenServer.call(pid, {:update_player, player})
  end

  @doc """
  Start the GenServer. Initial value can be a list of Players.
  """
  def start_link(initial_players) do
    GenServer.start_link(__MODULE__, initial_players)
  end

  def handle_call(:new_player, _from, players) do
    # Use the count of the players list to determine its Id.
    size = Enum.count(players)
    # Set the default x and y coordinates of the bird here.

    # TODO: The client should probably pass up the initial starting position.
    # Setting it at 300 to best approximate where the client Birds start off.
    new_player = %{server_id: size, x: 200, y: 300}
    {:reply, new_player, Map.put(players, new_player[:server_id], new_player)}
  end

  def handle_call(:get_players, _from, players) do
    {:reply, players, players}
  end

  def handle_call({:get_other_players, player}, _from, players) do
    other_players = Map.delete(players, player[:server_id])
    {:reply, other_players, players}
  end

  def handle_call({:get_num_players}, _from, players) do
    num_players = map_size(players)
    {:reply, num_players, players}
  end

  def handle_call({:get_player, server_id}, _from, players) do
    {:reply, players[server_id], players}
  end

  def handle_call({:update_player, player}, _from, players) do
    updated_players = Map.put(players, player[:server_id], player)
    {:reply, player, updated_players}
  end
end
