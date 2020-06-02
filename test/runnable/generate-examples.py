import os
import os.path as p

base_path = p.dirname(__file__)


def read_files():
    examples = {}
    ext_to_fieldname = {'.kan': 'code', '.expected': 'output'}

    for filepath in map(lambda f: p.join(base_path, f), os.listdir(base_path)):
        name, extension = p.splitext(filepath)
        if extension not in ext_to_fieldname:
            continue

        if p.isdir(name):
            continue

        if name not in examples:
            examples[name] = {}

        content = ''
        with open(filepath) as f:
            content = f.read()

        examples[name][ext_to_fieldname[extension]] = content

    return examples


def generate_markdown(examples):
    markdown = ''

    for filename in examples:
        example = examples[filename]
        comment = '\n'.join(map(lambda l: f'// {l}', example['output'].splitlines()))

        markdown += f'{filename}.kan:\n```\n// Outputs:\n{comment}\n{example["code"]}```\n'

    return markdown


if __name__ == '__main__':
    examples = read_files()
    print(generate_markdown(examples))
