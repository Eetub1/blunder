interface MessageProps {
    message: string,
    isError: boolean
}

const Message = ({ message, isError }: MessageProps) => {

    return (
        <div className="d-flex align-items-center justify-content-center" style={{
            "width": "100vw",
            "padding": "10px",
            "border": isError ? "2px solid red" : "2px solid green",
            "backgroundColor": "white",
            "borderRadius": "5px",
            "position": "fixed",
            "zIndex": 999
        }}>
            <p style={{"color": "black", "textAlign": "center", "margin": "0px"}}>{message}</p>
        </div>
    )
}

export default Message