defmodule Server.RoomChannel do
  use Server.Web, :channel

  def join("room:" <> room_id, _params, socket) do
    {:ok, assign(socket, :room_id, String.to_integer(room_id))}
  end

  def handle_in(event, params, socket) do
    nil |>
    handle_in(event, params, socket)
  end

  def handle_in(user, "hi", params, socket) do
    broadcast! socket, "hi", %{}
    {:noreply, socket}
  end
end
