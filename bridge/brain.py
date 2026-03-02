import sys
import os
import requests
import re

MODEL_ID = "deepseek-coder:6.7b"
API_URL = "http://localhost:11434/api/generate"

def get_command(user_prompt):
    # Captura del estado del sistema padre
    current_dir = os.getcwd().replace('\\', '/')

    SYSTEM_PROMPT = (
        "You are a strict Unix command translator running on Windows with Git Bash. "
        f"Current working directory: {current_dir}\n"
        "Convert natural language requests into raw Unix commands.\n"
        "RULES:\n"
        "- Output ONLY the command, nothing else.\n"
        "- No markdown, no backticks, no explanations.\n"
        "- Use standard Unix commands (ls, mkdir, rm, find, grep, cd).\n"
        "- For multiple operations, use && to chain.\n"
        "- Use 'mkdir -p' to avoid errors if directory exists.\n"
        "Translate this request:"
    )

    payload = {
        "model": MODEL_ID,
        "system": SYSTEM_PROMPT,
        "prompt": user_prompt,
        "stream": False,
        "options": {"temperature": 0.0}
    }

    try:
        response = requests.post(API_URL, json=payload)
        response.raise_for_status()
        raw_text = response.json()['response'].strip()

        code_match = re.search(r"```(?:bash|sh)?\s*(.*?)\s*```", raw_text, re.DOTALL)
        if code_match:
            command = code_match.group(1).strip()
        else:
            command = raw_text.strip()

        lines = [l.strip() for l in command.split('\n') if l.strip()]
        for line in lines:
            if re.match(r'^(to |sure|here|this|you|the |note:)', line, re.I):
                continue
            command = line
            break

        command = command.strip('`').strip()
        return command.split('\n')[0]

    except Exception as e:
        return f"echo 'Bridge Error: {str(e)}'"

if __name__ == "__main__":
    if len(sys.argv) > 1:
        prompt = " ".join(sys.argv[1:])
        print(get_command(prompt), end="")
    else:
        print("echo 'Error: No input'", end="")
