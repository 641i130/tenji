use std::error::Error;

fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = std::env::args().collect();
    let mut port = serialport::new("/dev/ttyUSB0", 9600).open().expect("Failed to open port");
    let command = args.get(1).expect("Please provide a command as the first argument");
    port.write(command.as_bytes())?;
    drop(port);
    Ok(())
}

