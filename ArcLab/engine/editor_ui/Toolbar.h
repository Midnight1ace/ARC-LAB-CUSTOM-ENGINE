#pragma once

namespace arclab {

enum class SimulationCommand {
    None,
    Play,
    Pause,
    Reset,
    Step,
};

class Toolbar {
public:
    void set_command(SimulationCommand command)
    {
        command_ = command;
    }

    SimulationCommand consume_command()
    {
        const SimulationCommand command = command_;
        command_ = SimulationCommand::None;
        return command;
    }

private:
    SimulationCommand command_ = SimulationCommand::None;
};

} // namespace arclab
