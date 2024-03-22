import Container from "@/app/_components/container";
import { EXAMPLE_PATH } from "@/lib/constants";

export function Footer() {
  return (
    <footer className="bg-neutral-50 border-t border-neutral-200">
      <Container>
        <div className="py-14 flex justify-center items-center">
            <a
              href="https://github.com/Torsion-Audio/nn-inference-template"
              className="font-bold hover:underline text-center" // Added text-center for explicit centering, might not be necessary but ensures centering in all scenarios
            >
              View on GitHub
            </a>
        </div>
      </Container>
    </footer>
  );
}

export default Footer;
